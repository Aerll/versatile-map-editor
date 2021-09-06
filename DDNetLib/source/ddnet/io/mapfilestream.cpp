#include <ddnet/io/mapfilestream.hpp>

#include <ddnet/util/macros.hpp>
#include <ddnet/util/concepts.hpp>
#include <ddnet/util/utility.hpp>

#include <QByteArrayList>
#include <QFile>
#include <QFileInfo>

#include <utility>
#include <zlib.h>

namespace ddnet::concepts::detail {

template <typename _Type>
concept ResizableContainer = requires(_Type type) {
    typename _Type::value_type;
    { type.data() } -> concepts::SameAs<typename _Type::value_type*>;
    type.resize(0);
};

} // ddnet::concepts::detail::



namespace ddnet::io {

debug::ErrorCode MAPFileStream::loadFile(const QFileInfo& file_info) {

#define UNCOMPRESS_(compressed_data_stream_, data_index_, destination_)                                 \
    if (debug::failed(uncompressData(compressed_data_stream_, data_index_, destination_))) [[unlikely]] \
        return debug::ErrorCode::ZLIB_DecompressionFailed



    QFile file{ file_info.absoluteFilePath() };
    
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) [[unlikely]]
        return debug::ErrorCode::File_FailedToRead;

    QDataStream map_data_stream{ &file };
    if (map_data_stream.device()->size() == 0) [[unlikely]]
        return debug::ErrorCode::File_MAP_NoData;



    // read the header
    map_data_stream.device()->reset();
    if (util::readRawData(map_data_stream, &data.header, sizeof(MAPHeader)) != sizeof(data.header)) [[unlikely]]
        return debug::ErrorCode::File_MAP_InvalidHeader;
    if (data.header.id != map::_id) [[unlikely]] // has to be "DATA"
        return debug::ErrorCode::File_MAP_InvalidSignature;
    if (data.header.version != map::_version) [[unlikely]] // support only the most recent version
        return debug::ErrorCode::File_MAP_UnsupportedVersion;



    // read the data
    auto _ReadFileInfoData = [&map_data_stream] <concepts::detail::ResizableContainer _Container> (
        _Container& container, qint32 count, debug::ErrorCode error_code)->debug::ErrorCode 
    {
        qint32 bytes;

        container.resize(count);
        bytes = count * sizeof(_Container::value_type);
        
        if (util::readRawData(map_data_stream, container.data(), bytes) != bytes) [[unlikely]]
            return error_code;
        return debug::ErrorCode::NoError;
    };

#define UNLIKELY_RETURN_ON_ERROR_()             \
    if (debug::failed(error_code)) [[unlikely]] \
        return error_code

    debug::ErrorCode error_code;
    error_code = _ReadFileInfoData(data.file_info.item_types,      data.header.item_types_count, debug::ErrorCode::File_MAP_FailedToLoadItemTypes     );
    UNLIKELY_RETURN_ON_ERROR_();

    error_code = _ReadFileInfoData(data.file_info.item_offsets,    data.header.items_count,      debug::ErrorCode::File_MAP_FailedToLoadItemOffsets   );
    UNLIKELY_RETURN_ON_ERROR_(); data.file_info.item_offsets_end = map_data_stream.device()->pos();

    error_code = _ReadFileInfoData(data.file_info.data_offsets,    data.header.raw_data_count,   debug::ErrorCode::File_MAP_FailedToLoadDataOffsets   );
    UNLIKELY_RETURN_ON_ERROR_(); data.file_info.data_offsets_end = map_data_stream.device()->pos();

    error_code = _ReadFileInfoData(data.file_info.data_sizes,      data.header.raw_data_count,   debug::ErrorCode::File_MAP_FailedToLoadDataSizes     );
    UNLIKELY_RETURN_ON_ERROR_();

    error_code = _ReadFileInfoData(data.file_info.item_data,       data.header.item_size,        debug::ErrorCode::File_MAP_FailedToLoadItemData      );
    UNLIKELY_RETURN_ON_ERROR_();

    error_code = _ReadFileInfoData(data.file_info.compressed_data, data.header.data_size,        debug::ErrorCode::File_MAP_FailedToLoadCompressedData);
    UNLIKELY_RETURN_ON_ERROR_();

    if (!map_data_stream.atEnd()) [[unlikely]] // not at the end of the stream
        return debug::ErrorCode::File_MAP_InvalidData;

    
    
    auto _IntsToBytes = [] <size_t _Size> (const std::array<qint32, _Size>& name, qint32 name_length, QByteArray& destination)->void {
        destination.reserve(name_length * sizeof(qint32));
        for (qint32 i = 0; i < name_length; ++i) {
            destination.push_back(((name[i] >> 24) & 0xff) - 128);
            destination.push_back(((name[i] >> 16) & 0xff) - 128);
            destination.push_back(((name[i] >> 8 ) & 0xff) - 128);
            destination.push_back(( name[i]        & 0xff) - 128);
        }
    };

    // read item types
#define RESERVE_(item_) item_.reserve(item_.size() + item_type.count)
#define FOR_EACH_ITEM_() for (qint32 item_offset_index = item_type.start; item_offset_index < item_type.start + item_type.count; ++item_offset_index)
#define READ_(item_, type_) {                                                        \
    type_ temp_item;                                                                 \
    item_data_stream.device()->seek(data.file_info.item_offsets[item_offset_index]); \
    util::readRawData(item_data_stream, &temp_item, sizeof(type_));                  \
    item_.push_back(std::move(temp_item));                                           \
    } FORCE_SEMICOLON_
#define READ_SEQUENCE_(item_, type_)                                                 \
    MAPDataFileItem data_file_item;                                                  \
    item_data_stream.device()->seek(data.file_info.item_offsets[item_offset_index]); \
    util::readRawData(item_data_stream, &data_file_item, sizeof(MAPDataFileItem));   \
    qint32 count = data_file_item.size / sizeof(type_);                              \
    item_.reserve(item_.size() + count);                                             \
    for (qint32 i = 0; i < count; ++i) {                                             \
        type_ temp_item;                                                             \
        util::readRawData(item_data_stream, &temp_item, sizeof(type_));              \
        item_.push_back(std::move(temp_item));                                       \
    } FORCE_SEMICOLON_

    
    MAPItemData item_data;
    QDataStream item_data_stream{ &data.file_info.item_data, QIODevice::ReadOnly };

    for (qint32 item_type_index = 0; item_type_index < data.file_info.item_types.size(); ++item_type_index) {
        const MAPItemType& item_type = data.file_info.item_types[item_type_index];
        
        switch (item_type.type) {
            case map::ItemType::Version: { // read the version
                RESERVE_(item_data.versions);
                FOR_EACH_ITEM_() {
                    READ_(item_data.versions, MAPItemVersion);
                
                    if (item_data.versions.back().version != map::_item_version) [[unlikely]]
                        return debug::ErrorCode::File_MAP_UnsupportedItemVersion;
                }
                break;
            }

            case map::ItemType::Info: { // read map details
                RESERVE_(item_data.infos);
                FOR_EACH_ITEM_() {
                    READ_(item_data.infos, MAPItemInfo);
                }
                break;
            }

            case map::ItemType::Image: { // read image assets
                RESERVE_(item_data.images);
                FOR_EACH_ITEM_() {
                    READ_(item_data.images, MAPItemImage);
                }
                break;
            }

            case map::ItemType::Envelope: { // read envelopes
                RESERVE_(item_data.envelopes);
                FOR_EACH_ITEM_() {
                    READ_(item_data.envelopes, MAPItemEnvelope);

                    if (item_data.envelopes.back().version < map::_item_envelope_version) [[unlikely]]
                        return debug::ErrorCode::File_MAP_UnsupportedItemEnvelopeVersion;
                }
                break;
            }

            case map::ItemType::Group: { // read groups
                RESERVE_(item_data.groups);
                FOR_EACH_ITEM_() {
                    READ_(item_data.groups, MAPItemGroup);

                    if (item_data.groups.back().version < map::_item_group_version) [[unlikely]]
                        return debug::ErrorCode::File_MAP_UnsupportedItemGroupVersion;
                }
                break;
            }

            case map::ItemType::Layer: { // read layers
                RESERVE_(item_data.layer_infos);
                RESERVE_(item_data.layers);
                FOR_EACH_ITEM_() {
                    READ_(item_data.layer_infos, MAPItemLayerInfo);

                    switch (item_data.layer_infos.back().type) {
                        case map::LayerType::Tile: {
                            READ_(item_data.layers, MAPItemLayerTile);
                            break;
                        }

                        case map::LayerType::Quad: {
                            READ_(item_data.layers, MAPItemLayerQuad);
                            break;
                        }

                        case map::LayerType::Sound: {
                            READ_(item_data.layers, MAPItemLayerSound);
                            break;
                        }

                        default:
                            return debug::ErrorCode::File_MAP_InvalidLayerType;
                    }
                }
                break;
            }

            case map::ItemType::EnvelopePoints: { // read envelope points
                FOR_EACH_ITEM_() {
                    READ_SEQUENCE_(item_data.envelope_points, MAPItemEnvelopePoint);
                }
                break;
            }

            case map::ItemType::Sound: { // read sound assets
                RESERVE_(item_data.sounds);
                FOR_EACH_ITEM_() {
                    READ_(item_data.sounds, MAPItemSound);
                }
                break;
            }
        }
    }



    // read the data
    QDataStream compressed_data_stream{ &data.file_info.compressed_data, QIODevice::ReadOnly };

    // read the details
    for (qint32 i = 0; i < item_data.infos.size(); ++i) {
        auto& item_info = item_data.infos[i];
        
        qint32 item_id = item_info.type_and_id & 0xFFFF;
        if (item_id != 0) [[unlikely]]
            continue;

        if (item_info.author_index >= 0)
            UNCOMPRESS_(compressed_data_stream, item_info.author_index, data.details.author);
        if (item_info.map_version_index >= 0)
            UNCOMPRESS_(compressed_data_stream, item_info.map_version_index, data.details.map_version);
        if (item_info.credits_index >= 0)
            UNCOMPRESS_(compressed_data_stream, item_info.credits_index, data.details.credits);
        if (item_info.license_index >= 0)
            UNCOMPRESS_(compressed_data_stream, item_info.license_index, data.details.license);

        if (item_info.version != 1) [[unlikely]]
            break;

        if (item_info.settings_index >= 0) {
            QByteArray settings_data;
            UNCOMPRESS_(compressed_data_stream, item_info.settings_index, settings_data);

            QByteArrayList commands_list = settings_data.split('\0'); // QByteArray is null terminated, split will create an additional empty command
            data.settings.assign(commands_list.begin(), commands_list.end() - 1); // don't copy it
        }
    }

    // read image assets
    for (qint32 i = 0; i < item_data.images.size(); ++i) {
        auto& item_image = item_data.images[i];

        ImageAsset image_asset;
        image_asset.is_external = item_image.is_external;
        image_asset.width = item_image.width;
        image_asset.height = item_image.height;

        UNCOMPRESS_(compressed_data_stream, item_image.image_name_index, image_asset.name);
        if (!image_asset.is_external)
            UNCOMPRESS_(compressed_data_stream, item_image.image_data_index, image_asset.data);

        data.assets.images.push_back(std::move(image_asset));
    }

    // read sound assets
    for (qint32 i = 0; i < item_data.sounds.size(); ++i) {
        auto& item_sound = item_data.sounds[i];

        SoundAsset sound_asset;
        sound_asset.is_external = item_sound.is_external;

        UNCOMPRESS_(compressed_data_stream, item_sound.sound_name_index, sound_asset.name);
        if (!sound_asset.is_external)
            UNCOMPRESS_(compressed_data_stream, item_sound.sound_data_index, sound_asset.data);

        data.assets.sounds.push_back(std::move(sound_asset));
    }

#define TEMP_BUFFER_ temp_buffer
#define TILES_DATA_STREAM_ tiles_data_stream
#define READ_TILES_(data_index_)                                                                     \
    QByteArray TEMP_BUFFER_;                                                                         \
    UNCOMPRESS_(compressed_data_stream, data_index_, TEMP_BUFFER_);                                  \
    QDataStream TILES_DATA_STREAM_{ &TEMP_BUFFER_, QIODevice::ReadOnly };                            \
    util::readRawData(TILES_DATA_STREAM_, layer_tile.tiles.data(), util::byteSize(layer_tile.tiles))
#define READ_TILES_INFO_(data_index_)                               \
    UNCOMPRESS_(compressed_data_stream, data_index_, TEMP_BUFFER_); \
    TILES_DATA_STREAM_.device()->reset()

    // read groups and layers
    for (qint32 i = 0; i < item_data.groups.size(); ++i) { // read group
        auto& item_group = item_data.groups[i];

        if (item_group.version != map::_item_group_version) [[unlikely]]
            return debug::ErrorCode::File_MAP_UnsupportedItemGroupVersion;

        Group group;
        _IntsToBytes(item_group.name, map::MAX_ITEM_GROUP_NAME_LENGTH, group.name);

        group.offset_x = item_group.offset_x;
        group.offset_y = item_group.offset_y;

        group.parallax_x = item_group.parallax_x;
        group.parallax_y = item_group.parallax_y;

        group.has_clipping = item_group.has_clipping;
        group.clip_x = item_group.clip_x;
        group.clip_y = item_group.clip_y;
        group.clip_w = item_group.clip_w;
        group.clip_h = item_group.clip_h;

        group.layers.reserve(item_group.layers_count);
        for (qint32 j = item_group.start_layer; j < item_group.start_layer + item_group.layers_count; ++j) {
            auto& item_layer_info = item_data.layer_infos[j];
            auto& item_layer = item_data.layers[j];

            switch (item_layer_info.type) {
                case map::LayerType::Tile: { // read tile layer
                    auto& item_layer_tile = std::get<MAPItemLayerTile>(item_layer);

                    if (item_layer_tile.version != map::_item_layer_tile_version) [[unlikely]]
                        return debug::ErrorCode::File_MAP_UnsupportedItemLayerTileVersion;
                    
                    LayerTile layer_tile;
                    _IntsToBytes(item_layer_tile.name, map::MAX_ITEM_LAYER_NAME_LENGTH, layer_tile.name);

                    layer_tile.asset_index = item_layer_tile.image_index;
                    layer_tile.is_detail = item_layer_info.is_detail;

                    layer_tile.width = item_layer_tile.width;
                    layer_tile.height = item_layer_tile.height;
                    layer_tile.special_type = item_layer_tile.special_type;

                    layer_tile.color = item_layer_tile.color;
                    layer_tile.color_envelope_index = item_layer_tile.color_envelope_index;
                    layer_tile.color_envelope_offset = item_layer_tile.color_envelope_offset;

                    layer_tile.tiles.resize(layer_tile.width * layer_tile.height);
                    switch (layer_tile.special_type) {
                        case map::SpecialLayerType::Tele: { // read tele layer
                            READ_TILES_(item_layer_tile.data_index);
                            READ_TILES_INFO_(item_layer_tile.tele_index);
                            for (qint32 tile_index = 0; tile_index < layer_tile.width * layer_tile.height; ++tile_index) {
                                TeleTileInfo tile_info;
                                util::readRawData(TILES_DATA_STREAM_, &tile_info, sizeof(TeleTileInfo));
                                layer_tile.tiles[tile_index].index = tile_info.index;
                                layer_tile.tiles_info.push_back(std::move(tile_info));
                            }
                            break;
                        }

                        case map::SpecialLayerType::Speedup: { // read speedup layer
                            READ_TILES_(item_layer_tile.data_index);
                            READ_TILES_INFO_(item_layer_tile.speedup_index);
                            for (qint32 tile_index = 0; tile_index < layer_tile.width * layer_tile.height; ++tile_index) {
                                SpeedupTileInfo tile_info;
                                util::readRawData(TILES_DATA_STREAM_, &tile_info, sizeof(SpeedupTileInfo));
                                layer_tile.tiles[tile_index].index = tile_info.index; // type stores the index, need to copy
                                layer_tile.tiles_info.push_back(std::move(tile_info));
                            }
                            break;
                        }

                        case map::SpecialLayerType::Front: { // read front layer
                            READ_TILES_(item_layer_tile.front_index);
                            break;
                        }

                        case map::SpecialLayerType::Switch: { // read switch layer
                            READ_TILES_(item_layer_tile.data_index);
                            READ_TILES_INFO_(item_layer_tile.switch_index);
                            for (qint32 tile_index = 0; tile_index < layer_tile.width * layer_tile.height; ++tile_index) {
                                SwitchTileInfo tile_info;
                                util::readRawData(TILES_DATA_STREAM_, &tile_info, sizeof(SwitchTileInfo));
                                layer_tile.tiles[tile_index].index = tile_info.index;
                                layer_tile.tiles_info.push_back(std::move(tile_info));
                            }
                            break; 
                        }

                        case map::SpecialLayerType::Tune: { // read tune layer
                            READ_TILES_(item_layer_tile.data_index);
                            READ_TILES_INFO_(item_layer_tile.tune_index);
                            for (qint32 tile_index = 0; tile_index < layer_tile.width * layer_tile.height; ++tile_index) {
                                TuneTileInfo tile_info;
                                util::readRawData(TILES_DATA_STREAM_, &tile_info, sizeof(TuneTileInfo));
                                layer_tile.tiles[tile_index].index = tile_info.index;
                                layer_tile.tiles_info.push_back(std::move(tile_info));
                            }
                            break;
                        }

                        case map::SpecialLayerType::Game: // read game layer
                        default: { // read design tile layer
                            READ_TILES_(item_layer_tile.data_index);
                        }
                    }

                    group.layers.push_back(std::move(layer_tile));
                    break;
                }

                case map::LayerType::Quad: { // read quad layer, these are little endian
                    auto& item_layer_quad = std::get<MAPItemLayerQuad>(item_layer);

                    if (item_layer_quad.version != map::_item_layer_quad_version) [[unlikely]]
                        return debug::ErrorCode::File_MAP_UnsupportedItemLayerQuadVersion;

                    LayerQuad layer_quad;
                    _IntsToBytes(item_layer_quad.name, map::MAX_ITEM_LAYER_NAME_LENGTH, layer_quad.name);

                    layer_quad.asset_index = item_layer_quad.image_index;
                    layer_quad.is_detail = item_layer_info.is_detail;

                    QByteArray temp_buffer;
                    UNCOMPRESS_(compressed_data_stream, item_layer_quad.data_index, temp_buffer);
                    QDataStream quads_data_stream{ &temp_buffer, QIODevice::ReadOnly };
                    quads_data_stream.setByteOrder(QDataStream::LittleEndian); // swap endianness

                    layer_quad.quads.resize(item_layer_quad.quads_count);
                    util::readRawData(quads_data_stream, layer_quad.quads.data(), util::byteSize(layer_quad.quads));

                    group.layers.push_back(std::move(layer_quad));
                    break;
                }

                case map::LayerType::Sound: { // read sound layer, these are little endian
                    auto& item_layer_sound = std::get<MAPItemLayerSound>(item_layer);

                    if (item_layer_sound.version != map::_item_layer_sound_version) [[unlikely]]
                        return debug::ErrorCode::File_MAP_UnsupportedItemLayerSoundVersion;

                    LayerSound layer_sound;
                    _IntsToBytes(item_layer_sound.name, map::MAX_ITEM_LAYER_NAME_LENGTH, layer_sound.name);

                    layer_sound.asset_index = item_layer_sound.sound_index;
                    layer_sound.is_detail = item_layer_info.is_detail;

                    QByteArray temp_buffer;
                    UNCOMPRESS_(compressed_data_stream, item_layer_sound.data_index, temp_buffer);
                    QDataStream sounds_data_stream{ &temp_buffer, QIODevice::ReadOnly };
                    sounds_data_stream.setByteOrder(QDataStream::LittleEndian); // swap endianness

                    layer_sound.sound_sources.reserve(item_layer_sound.sources_count);
                    for (qint32 source_index = 0; source_index < item_layer_sound.sources_count; ++source_index) {
                        SoundSource sound_source;
                        util::readRawData(sounds_data_stream, &sound_source, sizeof(SoundSource) - sizeof(SoundShape));
                        
                        sounds_data_stream >> sound_source.shape.type;
                        switch (sound_source.shape.type) {
                            case map::SoundSourceShapeType::Circle: {
                                util::CircleRadius circle_radius;
                                sounds_data_stream >> circle_radius;
                                sounds_data_stream.skipRawData(4);
                                sound_source.shape.size = circle_radius;
                                break;
                            }

                            case map::SoundSourceShapeType::Rectangle: {
                                util::RectangleSize rectangle_size;
                                sounds_data_stream >> rectangle_size.width;
                                sounds_data_stream >> rectangle_size.height;
                                sound_source.shape.size = rectangle_size;
                                break;
                            }
                        }
                        layer_sound.sound_sources.push_back(std::move(sound_source));
                    }

                    group.layers.push_back(std::move(layer_sound));
                    break;
                }
            }
        }

        data.groups.push_back(std::move(group));
    }

    // read the envelopes
    for (qint32 i = 0; i < item_data.envelopes.size(); ++i) {
        auto& item_envelope = item_data.envelopes[i];

        Envelope envelope;
        if (item_envelope.name[0] != -1) // compatibility with old maps
            _IntsToBytes(item_envelope.name, map::MAX_ITEM_ENVELOPE_NAME_LENGTH, envelope.name);

        envelope.channels = item_envelope.channels;
        envelope.is_synchronized = item_envelope.is_synchronized;

        envelope.points.reserve(item_envelope.points_count); // read envelope points
        for (qint32 j = item_envelope.start_point; j < item_envelope.start_point + item_envelope.points_count; ++j) {
            auto& item_envelope_point = item_data.envelope_points[j];

            EnvelopePoint envelope_point;
            envelope_point.time = item_envelope_point.time;
            envelope_point.curve_type = item_envelope_point.curve_type;
            envelope_point.values = std::move(item_envelope_point.values);
            envelope.points.push_back(std::move(envelope_point));
        }
        data.envelopes.push_back(std::move(envelope));
    }
    return debug::ErrorCode::NoError;
}



debug::ErrorCode MAPFileStream::saveFile(const QFileInfo& file_info) {

#define DATAS_ datas
#define TEMP_BUFFER_ temp_buffer
#define COMPRESS_(source_)                                               \
    if (debug::failed(compressData(source_, TEMP_BUFFER_))) [[unlikely]] \
        return debug::ErrorCode::ZLIB_CompressionFailed;                 \
    DATAS_.push_back(std::move(TEMP_BUFFER_))
#define ADD_UNCOMPRESSED_SIZE_(name_, size_)   \
    qint32 name_ = size_;                      \
    data.file_info.data_sizes.push_back(name_)



    QFile file{ file_info.absoluteFilePath() };
    
    file.open(QIODevice::WriteOnly);
    if (!file.isOpen()) [[unlikely]]
        return debug::ErrorCode::File_FailedToWrite;

    data.file_info.item_types.clear();
    data.file_info.item_offsets.clear();
    data.file_info.data_offsets.clear();
    data.file_info.data_sizes.clear();
    data.file_info.item_data.clear();
    data.file_info.compressed_data.clear();

    QDataStream map_data_stream{ &file };

    std::vector<QByteArray> items;
    std::vector<QByteArray> DATAS_;
    QByteArray TEMP_BUFFER_;
    items.reserve(512);
    datas.reserve(512);

    std::vector<std::pair<map::ItemType, qint32>> item_type_counts;
    item_type_counts.reserve(8);

    auto _ToQByteArray = [] <typename _Type> (const _Type& value) {
        return QByteArray{ reinterpret_cast<const char*>(&value), sizeof(_Type) };
    };

    auto _AddItem = [&] <typename _Type> (const _Type& value)->void {
        if constexpr (concepts::SameAs<_Type, QByteArray>)
            items.push_back(value);
        else
            items.emplace_back(_ToQByteArray(value));
    };

    auto _AddItemType = [&](map::ItemType type, qint32 count) {
        item_type_counts.emplace_back(std::pair<map::ItemType, qint32>{ type, count });
    };

    // save the version
    {
        MAPItemVersion item_version;
        item_version.type_and_id = (map::ItemType::Version << 16);
        item_version.size = sizeof(MAPItemVersion) - sizeof(MAPDataFileItem);
        item_version.version = map::_item_version;
        _AddItem(item_version);
        _AddItemType(map::ItemType::Version, 1);
    }

    // save the details
    {
        MAPItemInfo item_info;
        item_info.type_and_id = (map::ItemType::Info << 16);
        item_info.size = sizeof(MAPItemInfo) - sizeof(MAPDataFileItem);
        item_info.version = map::_item_version;
        item_info.author_index = -1;
        item_info.map_version_index = -1;
        item_info.credits_index = -1;
        item_info.license_index = -1;
        item_info.settings_index = -1;

        if (!data.details.author.isEmpty()) {
            ADD_UNCOMPRESSED_SIZE_(uncompressed_size, data.details.author.size());
            COMPRESS_(data.details.author);
            item_info.author_index = DATAS_.size() - 1;
        }
        if (!data.details.map_version.isEmpty()) {
            ADD_UNCOMPRESSED_SIZE_(uncompressed_size, data.details.map_version.size());
            COMPRESS_(data.details.map_version);
            item_info.map_version_index = DATAS_.size() - 1;
        }
        if (!data.details.credits.isEmpty()) {
            ADD_UNCOMPRESSED_SIZE_(uncompressed_size, data.details.credits.size());
            COMPRESS_(data.details.credits);
            item_info.credits_index = DATAS_.size() - 1;
        }
        if (!data.details.license.isEmpty()) {
            ADD_UNCOMPRESSED_SIZE_(uncompressed_size, data.details.license.size());
            COMPRESS_(data.details.license);
            item_info.license_index = DATAS_.size() - 1;
        }
        if (!data.settings.empty()) {
            ADD_UNCOMPRESSED_SIZE_(uncompressed_size, util::totalSize(data.settings) + data.settings.size());
            QByteArray commands;
            commands.reserve(uncompressed_size);
            qint32 test = commands.capacity();
            for (const auto& command : data.settings) {
                commands.push_back(command);
                commands.push_back('\0');
            }
            COMPRESS_(commands);
            item_info.settings_index = DATAS_.size() - 1;
        }
        _AddItem(item_info);
        _AddItemType(map::ItemType::Info, 1);
    }
    
    // save image assets
    for (qint32 i = 0; i < data.assets.images.size(); ++i) {
        const auto& image_asset = data.assets.images[i];

        MAPItemImage item_image;
        item_image.type_and_id = (map::ItemType::Image << 16) | i;
        item_image.size = sizeof(MAPItemImage) - sizeof(MAPDataFileItem);
        item_image.version = map::_item_version;
        item_image.width = image_asset.width;
        item_image.height = image_asset.height;
        item_image.is_external = image_asset.is_external;

        ADD_UNCOMPRESSED_SIZE_(name_uncompressed_size, image_asset.name.size());
        COMPRESS_(image_asset.name);
        item_image.image_name_index = DATAS_.size() - 1;

        if (image_asset.is_external)
            item_image.image_data_index = -1;
        else {
            ADD_UNCOMPRESSED_SIZE_(data_uncompressed_size, image_asset.data.size());
            COMPRESS_(image_asset.data);
            item_image.image_data_index = DATAS_.size() - 1;
        }
        _AddItem(item_image);
    }
    _AddItemType(map::ItemType::Image, data.assets.images.size());

    // save sound assets
    for (qint32 i = 0; i < data.assets.sounds.size(); ++i) {
        const auto& sound_asset = data.assets.sounds[i];

        MAPItemSound item_sound;
        item_sound.type_and_id = (map::ItemType::Sound << 16) | i;
        item_sound.size = sizeof(MAPItemSound) - sizeof(MAPDataFileItem);
        item_sound.version = map::_item_version;
        item_sound.is_external = sound_asset.is_external;

        ADD_UNCOMPRESSED_SIZE_(name_uncompressed_size, sound_asset.name.size());
        COMPRESS_(sound_asset.name);
        item_sound.sound_name_index = DATAS_.size() - 1;

        if (sound_asset.is_external) {
            item_sound.sound_data_index = -1;
            item_sound.sound_data_size = 0;
        }
        else {
            ADD_UNCOMPRESSED_SIZE_(data_uncompressed_size, sound_asset.data.size());
            COMPRESS_(sound_asset.data);
            item_sound.sound_data_index = DATAS_.size() - 1;
            item_sound.sound_data_size = data_uncompressed_size;
        }
        _AddItem(item_sound);
    }
    _AddItemType(map::ItemType::Sound, data.assets.sounds.size());



    auto _BytesToInts = [] <size_t _Size> (const QByteArray& name, std::array<qint32, _Size>& destination)->void {
        QByteArray buffer = name;
        buffer.resize(destination.size() * sizeof(qint32));
        for (qint32 i = 0; i < destination.size(); ++i)
            destination[i] = ((buffer[static_cast<qint32>(i * sizeof(qint32))]     + 128) << 24)
                           | ((buffer[static_cast<qint32>(i * sizeof(qint32) + 1)] + 128) << 16)
                           | ((buffer[static_cast<qint32>(i * sizeof(qint32) + 2)] + 128) << 8)
                           |  (buffer[static_cast<qint32>(i * sizeof(qint32) + 3)] + 128);
        destination.back() &= 0xffffff00;
    };

#define SAVE_TILE_INFO_BEGIN_(buffer_name_, name_, type_)                                          \
    QByteArray buffer_name_;                                                                       \
    buffer_name_.reserve(layer_tile.width * layer_tile.height * sizeof(type_));                    \
    for (qint32 tile_index = 0; tile_index < layer_tile.width * layer_tile.height; ++tile_index) { \
        const auto& name_ = std::get<type_>(layer_tile.tiles_info[tile_index])
#define SAVE_TILE_INFO_END_(buffer_name_, data_index_)                            \
    }                                                                             \
        ADD_UNCOMPRESSED_SIZE_(tile_info_uncompressed_size, buffer_name_.size()); \
        COMPRESS_(buffer_name_);                                                  \
        data_index_ = DATAS_.size() - 1

    // save groups
    {
        quint32 layer_count = 0;
        for (qint32 i = 0; i < data.groups.size(); ++i) {
            const auto& group = data.groups[i];

            MAPItemGroup item_group;
            _BytesToInts(group.name, item_group.name);

            item_group.type_and_id = (map::ItemType::Group << 16) | i;
            item_group.size = sizeof(MAPItemGroup) - sizeof(MAPDataFileItem);
            item_group.version = map::_item_group_version;

            item_group.offset_x = group.offset_x;
            item_group.offset_y = group.offset_y;
            item_group.parallax_x = group.parallax_x;
            item_group.parallax_y = group.parallax_y;

            item_group.start_layer = layer_count;
            item_group.layers_count = group.layers.size();
            item_group.has_clipping = group.has_clipping;

            item_group.clip_x = group.clip_x;
            item_group.clip_y = group.clip_y;
            item_group.clip_w = group.clip_w;
            item_group.clip_h = group.clip_h;

            layer_count += group.layers.size();
            _AddItem(item_group);
        }
        _AddItemType(map::ItemType::Group, data.groups.size());
    }

    // save layers
    {
        quint32 layer_count = 0;
        for (qint32 i = 0; i < data.groups.size(); ++i) {
            const auto& group = data.groups[i];

            for (qint32 j = 0; j < group.layers.size(); ++j, ++layer_count) {
                const auto& layer_variant = group.layers[j];

                MAPItemLayerInfo item_layer_info;
                item_layer_info.type_and_id = (map::ItemType::Layer << 16) | layer_count;
                item_layer_info.version = 0;

                switch (layer_variant.index()) {
                    case 0: { // tile layer
                        const auto& layer_tile = std::get<LayerTile>(layer_variant);

                        MAPItemLayerTile item_layer_tile;
                        _BytesToInts(layer_tile.name, item_layer_tile.name);

                        item_layer_tile.info = std::move(item_layer_info);
                        item_layer_tile.info.size = sizeof(MAPItemLayerTile) - sizeof(MAPDataFileItem);
                        item_layer_tile.info.type = map::LayerType::Tile;
                        item_layer_tile.info.is_detail = layer_tile.is_detail;
                        item_layer_tile.version = map::_item_layer_tile_version;
                        item_layer_tile.width = layer_tile.width;
                        item_layer_tile.height = layer_tile.height;
                        item_layer_tile.special_type = layer_tile.special_type;

                        item_layer_tile.color = layer_tile.color;
                        item_layer_tile.color_envelope_index = layer_tile.color_envelope_index;
                        item_layer_tile.color_envelope_offset = layer_tile.color_envelope_offset;
                        item_layer_tile.image_index = layer_tile.asset_index;

                        QByteArray tile_buffer = QByteArray::fromRawData(reinterpret_cast<const char*>(layer_tile.tiles.data()), util::byteSize(layer_tile.tiles));
                        ADD_UNCOMPRESSED_SIZE_(tile_uncompressed_size, tile_buffer.size());
                        COMPRESS_(tile_buffer);
                        item_layer_tile.data_index = DATAS_.size() - 1;

                        item_layer_tile.tele_index = -1;
                        item_layer_tile.speedup_index = -1;
                        item_layer_tile.front_index = -1;
                        item_layer_tile.switch_index = -1;
                        item_layer_tile.tune_index = -1;

                        switch (layer_tile.special_type) {
                            case map::SpecialLayerType::Tele: {
                                SAVE_TILE_INFO_BEGIN_(tile_info_buffer, tile_info, TeleTileInfo);
                                tile_info_buffer.push_back(tile_info.number);
                                tile_info_buffer.push_back(tile_info.index);
                                SAVE_TILE_INFO_END_(tile_info_buffer, item_layer_tile.tele_index);
                                break;
                            }
                                                            
                            case map::SpecialLayerType::Speedup: {
                                SAVE_TILE_INFO_BEGIN_(tile_info_buffer, tile_info, SpeedupTileInfo);
                                tile_info_buffer.push_back(tile_info.force);
                                tile_info_buffer.push_back(tile_info.max_speed);
                                tile_info_buffer.push_back(tile_info.index);
                                tile_info_buffer.push_back('\0'); // padding
                                tile_info_buffer.push_back(static_cast<char>(tile_info.angle));
                                tile_info_buffer.push_back(static_cast<char>((tile_info.angle & 0xFF00) >> 8));
                                SAVE_TILE_INFO_END_(tile_info_buffer, item_layer_tile.speedup_index);
                                break;
                            }

                            case map::SpecialLayerType::Front: {
                                item_layer_tile.front_index = item_layer_tile.data_index;
                                break;
                            }

                            case map::SpecialLayerType::Switch: {
                                SAVE_TILE_INFO_BEGIN_(tile_info_buffer, tile_info, SwitchTileInfo);
                                tile_info_buffer.push_back(tile_info.number);
                                tile_info_buffer.push_back(tile_info.index);
                                tile_info_buffer.push_back(tile_info.flags);
                                tile_info_buffer.push_back(tile_info.delay);
                                SAVE_TILE_INFO_END_(tile_info_buffer, item_layer_tile.switch_index);
                                break;
                            }

                            case map::SpecialLayerType::Tune: {
                                SAVE_TILE_INFO_BEGIN_(tile_info_buffer, tile_info, TuneTileInfo);
                                tile_info_buffer.push_back(tile_info.number);
                                tile_info_buffer.push_back(tile_info.index);
                                SAVE_TILE_INFO_END_(tile_info_buffer, item_layer_tile.tune_index);
                                break;
                            }
                        }
                        _AddItem(item_layer_tile);
                        break;
                    }

                    case 1: { // quad layer
                        const auto& layer_quad = std::get<LayerQuad>(layer_variant);

                        MAPItemLayerQuad item_layer_quad;
                        _BytesToInts(layer_quad.name, item_layer_quad.name);

                        item_layer_quad.info = std::move(item_layer_info);
                        item_layer_quad.info.size = sizeof(MAPItemLayerQuad) - sizeof(MAPDataFileItem);
                        item_layer_quad.info.type = map::LayerType::Quad;
                        item_layer_quad.info.is_detail = layer_quad.is_detail;
                        item_layer_quad.version = map::_item_layer_quad_version;
                        item_layer_quad.quads_count = layer_quad.quads.size();
                        item_layer_quad.image_index = layer_quad.asset_index;
                        item_layer_quad.data_index = -1;

                        if (!layer_quad.quads.empty()) {
                            QByteArray quad_buffer;

                            QDataStream quads_data_stream{ &quad_buffer, QIODevice::ReadWrite };
                            quads_data_stream.setByteOrder(QDataStream::LittleEndian); // swap endianness

                            util::writeRawData(quads_data_stream, layer_quad.quads.data(), util::byteSize(layer_quad.quads));

                            ADD_UNCOMPRESSED_SIZE_(quad_uncompressed_size, quad_buffer.size());
                            COMPRESS_(quad_buffer);
                            item_layer_quad.data_index = DATAS_.size() - 1;
                        }
                        _AddItem(item_layer_quad);
                        break;
                    }

                    case 2: { // sound layer
                        const auto& layer_sound = std::get<LayerSound>(layer_variant);

                        MAPItemLayerSound item_layer_sound;
                        _BytesToInts(layer_sound.name, item_layer_sound.name);

                        item_layer_sound.info = std::move(item_layer_info);
                        item_layer_sound.info.size = sizeof(MAPItemLayerSound) - sizeof(MAPDataFileItem);
                        item_layer_sound.info.type = map::LayerType::Sound;
                        item_layer_sound.info.is_detail = layer_sound.is_detail;
                        item_layer_sound.version = map::_item_layer_sound_version;
                        item_layer_sound.sources_count = layer_sound.sound_sources.size();
                        item_layer_sound.sound_index = layer_sound.asset_index;
                        item_layer_sound.data_index = -1;

                        if (!layer_sound.sound_sources.empty()) {
                            QByteArray sound_buffer;
                            sound_buffer.reserve(layer_sound.sound_sources.size() * (sizeof(SoundSource) - 4));

                            QDataStream sources_data_stream{ &sound_buffer, QIODevice::ReadWrite };
                            sources_data_stream.setByteOrder(QDataStream::LittleEndian); // swap endianness

                            for (const auto& sound_source : layer_sound.sound_sources) {
                                util::writeRawData(sources_data_stream, &sound_source, sizeof(SoundSource) - sizeof(SoundShape));
                                sources_data_stream << sound_source.shape.type;
                                switch (sound_source.shape.type) {
                                    case map::SoundSourceShapeType::Circle: {
                                        auto radius = std::get<util::CircleRadius>(sound_source.shape.size);
                                        sources_data_stream << radius << qint32(0);
                                        break;
                                    }

                                    case map::SoundSourceShapeType::Rectangle: {
                                        auto size = std::get<util::RectangleSize>(sound_source.shape.size);
                                        sources_data_stream << size.width << size.height;
                                        break;
                                    }
                                }
                            }

                            ADD_UNCOMPRESSED_SIZE_(quad_uncompressed_size, sound_buffer.size());
                            COMPRESS_(sound_buffer);
                            item_layer_sound.data_index = DATAS_.size() - 1;
                        }
                        _AddItem(item_layer_sound);
                        break;
                    }
                }
            }
        }
        _AddItemType(map::ItemType::Layer, layer_count);
    }

    // save envelopes
    {
        qint32 point_count = 0;
        for (qint32 i = 0; i < data.envelopes.size(); ++i) {
            const auto& envelope = data.envelopes[i];

            MAPItemEnvelope item_envelope;
            _BytesToInts(envelope.name, item_envelope.name);

            item_envelope.type_and_id = (map::ItemType::Envelope << 16) | i;
            item_envelope.size = sizeof(MAPItemEnvelope) - sizeof(MAPDataFileItem);
            item_envelope.version = map::_item_envelope_version;
            item_envelope.channels = envelope.channels;
            item_envelope.start_point = point_count;
            item_envelope.points_count = envelope.points.size();
            item_envelope.is_synchronized = envelope.is_synchronized;

            point_count += envelope.points.size();
            _AddItem(item_envelope);
        }
        _AddItemType(map::ItemType::Envelope, data.envelopes.size());

    // save envelope points
        MAPDataFileItem data_file_item;
        data_file_item.type_and_id = (map::ItemType::EnvelopePoints << 16);
        data_file_item.size = sizeof(MAPItemEnvelopePoint) * point_count;

        QByteArray temp_buffer;
        temp_buffer.reserve(sizeof(MAPDataFileItem) + sizeof(MAPItemEnvelopePoint) * point_count);
        temp_buffer.push_back(_ToQByteArray(data_file_item));

        for (qint32 i = 0; i < data.envelopes.size(); ++i) {
            const auto& envelope = data.envelopes[i];
            temp_buffer.push_back(QByteArray::fromRawData(reinterpret_cast<const char*>(envelope.points.data()), util::byteSize(envelope.points)));
        }
        _AddItem(temp_buffer);
        _AddItemType(map::ItemType::EnvelopePoints, 1);
    }



    // save to a file
    { // create item types
        qint32 current = 0;
        for (qint32 i = 0; i < item_type_counts.size(); ++i) {
            auto item_type_count = item_type_counts[i];
            data.file_info.item_types.emplace_back(MAPItemType{ .type = item_type_count.first, .start = current, .count = item_type_count.second });
            current += item_type_count.second;
        }
    }

    { // calculate item offsets
        data.file_info.item_offsets.reserve(items.size());
        qint32 item_offset = 0;
        for (qint32 i = 0; i < items.size(); ++i) {
            const auto& current_item = items[i];
            data.file_info.item_offsets.push_back(item_offset);
            item_offset += current_item.size();
        }
    }

    { // calculate data offsets
        data.file_info.data_offsets.reserve(DATAS_.size());
        qint32 data_offset = 0;
        for (qint32 i = 0; i < DATAS_.size(); ++i) {
            const auto& current_data = DATAS_[i];
            data.file_info.data_offsets.push_back(data_offset);
            data_offset += current_data.size();
        }
    }

    // write header
    qint32 data_total_size = util::totalSize(DATAS_);
    qint32 item_data_total_size = util::totalSize(items);
    qint32 file_size = sizeof(MAPHeader) + item_data_total_size + util::byteSize(data.file_info.item_offsets) + util::byteSize(data.file_info.item_types) + data_total_size + util::byteSize(data.file_info.data_sizes) * 2;
    data.header.id = map::_id; // DATA
    data.header.version = map::_version;
    data.header.size = file_size - 16;
    data.header.swaplen = file_size - data_total_size - 16;
    data.header.item_types_count = data.file_info.item_types.size();
    data.header.items_count = items.size();
    data.header.raw_data_count = DATAS_.size();
    data.header.item_size = item_data_total_size;
    data.header.data_size = data_total_size;

    util::writeRawData(map_data_stream, &data.header, sizeof(MAPHeader));

    // write file info
    data.file_info.item_data.reserve(item_data_total_size);
    data.file_info.compressed_data.reserve(data_total_size);

    for (qint32 i = 0; i < items.size(); ++i)
        data.file_info.item_data.push_back(items[i]);
    for (qint32 i = 0; i < DATAS_.size(); ++i)
        data.file_info.compressed_data.push_back(DATAS_[i]);

    util::writeRawData(map_data_stream, data.file_info.item_types.data(), util::byteSize(data.file_info.item_types));
    util::writeRawData(map_data_stream, data.file_info.item_offsets.data(), util::byteSize(data.file_info.item_offsets));
    util::writeRawData(map_data_stream, data.file_info.data_offsets.data(), util::byteSize(data.file_info.data_offsets));
    util::writeRawData(map_data_stream, data.file_info.data_sizes.data(), util::byteSize(data.file_info.data_sizes));
    util::writeRawData(map_data_stream, data.file_info.item_data.data(), data.file_info.item_data.size());
    util::writeRawData(map_data_stream, data.file_info.compressed_data.data(), data.file_info.compressed_data.size());

    return debug::ErrorCode::NoError;
}



qint32 MAPFileStream::itemSize(qint32 item_offset_index) const {
    const auto& item_offsets = data.file_info.item_offsets;
    auto item_offsets_end = data.file_info.item_offsets_end;

    if (item_offset_index == item_offsets.size() - 1)
        return item_offsets_end - item_offsets[item_offset_index];
    return item_offsets[item_offset_index + 1] - item_offsets[item_offset_index];
}



qint32 MAPFileStream::dataSize(qint32 data_offset_index) const {
    const auto& data_offsets = data.file_info.data_offsets;

    if (data_offset_index == data_offsets.size() - 1)
        return data.header.data_size - data_offsets[data_offset_index];
    return data_offsets[data_offset_index + 1] - data_offsets[data_offset_index];
}



debug::ErrorCode MAPFileStream::uncompressData(QDataStream& compressed_data_stream, qint32 data_index, QByteArray& destination) const {
    qint32 data_size = dataSize(data_index);

    QByteArray compressed_data;
    compressed_data.resize(data_size);

    compressed_data_stream.device()->seek(data.file_info.data_offsets[data_index]);
    util::readRawData(compressed_data_stream, compressed_data.data(), data_size);

    destination.resize(data.file_info.data_sizes[data_index]);
    uLong dest_size = destination.size();
    qint32 result = uncompress(
        reinterpret_cast<Bytef*>(destination.data()),
        reinterpret_cast<uLongf*>(&dest_size),
        reinterpret_cast<const Bytef*>(compressed_data.constData()),
        static_cast<uLong>(compressed_data.size())
    );
    if (result != Z_OK) [[unlikely]]
        return debug::ErrorCode::ZLIB_DecompressionFailed;
    return debug::ErrorCode::NoError;
}



debug::ErrorCode MAPFileStream::compressData(const QByteArray& source, QByteArray& destination) const {
    uLong bound = compressBound(source.size());

    destination.resize(bound);
    uLong dest_size = destination.size();

    qint32 result = compress2(
        reinterpret_cast<Bytef*>(destination.data()),
        reinterpret_cast<uLongf*>(&dest_size),
        reinterpret_cast<const Bytef*>(source.constData()),
        static_cast<uLong>(source.size()), 9
    );
    if (result != Z_OK) [[unlikely]]
        return debug::ErrorCode::ZLIB_CompressionFailed;
    destination.resize(dest_size);
    return debug::ErrorCode::NoError;
}

} // ddnet::io::
