#pragma once

#include <ddnet/io/mapdata.hpp>
#include <ddnet/debug/error_codes.hpp>

class QFileInfo;

namespace ddnet::io {

class MAPFileStream {
public:
    debug::ErrorCode loadFile(const QFileInfo& file_info);
    debug::ErrorCode saveFile(const QFileInfo& file_info);

private:
    qint32 itemSize(qint32 item_offset_index) const;
    qint32 dataSize(qint32 data_offset_index) const;
    debug::ErrorCode uncompressData(QDataStream& compressed_data_stream, qint32 data_index, QByteArray& destination) const;
    debug::ErrorCode compressData(const QByteArray& source, QByteArray& destination) const;

public:
    MAPData data;
};

/*
                    MAP FILE STRUCTURE

***********************************************************
* 
* The header.
* 
* Fields:
* id -> 0x41544144u.
* version -> 4.
* size -> file size - 16 bytes.
* swaplen -> file size - data size - 16 bytes.
* item_types_count -> number of item types.
* items_count -> number of items.
* raw_data_count -> number of data offsets and data sizes.
* item_size -> size of the item data.
* data_size -> size of the compressed data.
* 
***********************************************************
MAPHeader (36 bytes)
    \
    qint32 id;
    qint32 version;
    qint32 size;
    qint32 swaplen;
    qint32 item_types_count;
    qint32 items_count;
    qint32 raw_data_count;
    qint32 item_size;
    qint32 data_size;



***********************************************************
* 
* An array of item types. The number of entries is given by
* item_types_count.
* 
* Fields:
* type -> one of the following:
*     - Version
*     - Info
*     - Image
*     - Envelope
*     - Group
*     - Layer
*     - EnvelopePoints
*     - Sound
* start -> indicates the byte, where the items are located.
* count -> number of items to read.
* 
***********************************************************
MAPItemType array -> item_types_count
    \
    ItemType type;
    qint32 start;
    qint32 count;



***********************************************************
*
* An array of item offsets. The number of entries is given
* by items_count.
*
***********************************************************
qint32 array -> items_count



***********************************************************
*
* An array of compressed data offsets. The number of 
* entries is given by raw_data_count.
*
***********************************************************
qint32 array -> raw_data_count



***********************************************************
*
* An array of uncompressed data sizes. The number of 
* entries is given by raw_data_count.
*
***********************************************************
qint32 array -> raw_data_count



***********************************************************
*
* Item data. The size is given by item_size.
*
***********************************************************
byte array -> item_size



***********************************************************
*
* Compressed data. The size is given by data_size.
*
***********************************************************
byte array -> data_size

*/





/*
                  ITEM DATA SPECIFICATION

***********************************************************
*
* Item data.
* 
* Fields:
* type -> one of the following:
*     - Version
*     - Info
*     - Image
*     - Envelope
*     - Group
*     - Layer
*     - EnvelopePoints
*     - Sound
* 
* versions -> version of the item. 
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 1.
* 
* infos -> general map details.
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 1.
*     author_index -> index of the data holding author.
*     map_version_index -> index of the data holding map
*                          version.
*     credits_index -> index of the data holding credits.
*     license_index -> index of the data holding license.
*     settings_index -> index of the data holding server
*                       commands.
* 
* images -> image assets.
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 1.
*     width -> image width.
*     height -> image height.
*     is_external -> 1 - external, 0 - embedded.
*     image_name_index -> index of the data holding asset
*                         name.
*     image_data_index -> index of the data holding image.
*                         -1 if is_external is 1.
* 
* sounds -> sound assets.
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 1.
*     is_external -> 1 - external, 0 - embedded.
*     sound_name_index -> index of the data holding asset
*                         name.
*     sound_data_index -> index of the data holding sound.
*                         -1 if is_external is 1.
*     sound_data_size -> index of the data holding 
*                        uncompressed size. 0 if
*                        is_external is 1.
* 
* envelopes -> envelopes.
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 2.
*     channels -> number of all channels.
*     start_point -> index of the first envelope point.
*     points_count -> number of envelope points.
*     name -> envelope name.
*     is_synchronized -> 1 - synchronized,
*                        0 - not synchronized.
* 
* envelope_points -> envelope points.
*     Fields:
*     time -> time in milliseconds.
*     curve_type -> one of the following:
*         - Step
*         - Linear
*         - Slow
*         - Fast
*         - Smooth
*     values -> envelope channel values.
* 
* groups -> groups.
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 3.
*     offset_x -> horizontal offset for group layers.
*     offset_y -> vertical offset for group layers.
*     parallax_x -> horizontal parallax for group layers.
*     parallax_y -> vertical parallax for group layers.
*     start_layer -> index of the first layer.
*     layers_count -> number of layers.
*     has_clipping -> 1 - clipping, 0 - no clipping.
*     clip_x -> position x of the clipping rectangle.
*     clip_y -> position y of the clipping rectangle.
*     clip_w -> width of the clipping rectangle.
*     clip_h -> height of the clipping rectangle.
*     name -> group name.
* 
* layer_infos -> layer details.
*     Fields:
*     type_and_id -> (type << 16) | id.
*     size -> size of the following fields.
*     version -> 1.
*     type -> one of the following:
*         - Invalid
*         - Game
*         - Tile
*         - Quad
*         - Front
*         - Tele
*         - Speedup
*         - Switch
*         - Tune
*         - Sound
*     is_detail -> 1 - detail, 0 - not detail.
* 
* layers -> one of the following:
*     MAPItemLayerTile -> tile layer.
*         Fields:
*         info -> same as layer_infos.
*         version -> 3.
*         width -> layer width.
*         height -> layer height.
*         special_type -> one of the following:
*             - None
*             - Game
*             - Tele
*             - Speedup
*             - Front
*             - Switch
*             - Tune
*         color -> rgba color.
*         color_envelope_index -> index of the color
*                                 envelope. -1 if unused.
*         color_envelope_offset -> time offset in 
*                                  milliseconds.
*         image_index -> index of the image asset. -1 if no
*                        image is used.
*         data_index -> index of the data holding layer.
*         name -> layer name.
*         tele_index -> index of the data holding tele 
*                       layer. Valid only when special_type
*                       is Tele.
*         speedup_index -> index of the data holding 
*                          speedup layer. Valid only when
*                          special_type is Speedup.
*         front_index -> index of the data holding front
*                        layer. Valid only when
*                        special_type is Front.
*         switch_index -> index of the data holding switch
*                         layer. Valid only when
*                         special_type is Switch.
*         tune_index -> index of the data holding tune 
*                       layer. Valid only when special_type
*                       is Tune.
*     MAPItemLayerQuad -> quad layer.
*         Fields:
*         info -> same as layer_infos.
*         version -> 2.
*         quads_count -> number of quads.
*         data_index -> index of the data holding layer.
*         image_index -> index of the image asset. -1 if no
*                        image is used.
*         name -> layer name.
*     MAPItemLayerSound -> sound layer.
*         Fields:
*         info -> same as layer_infos.
*         version -> 2.
*         sources_count -> number of sound sources.
*         data_index -> index of the data holding layer.
*         sound_index -> index of the sound asset. -1 if no
*                        sound is used.
*         name -> layer name.
*
***********************************************************
MAPItemData
    \
    MAPItemType type;
    std::vector<MAPItemVersion> versions;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
    std::vector<MAPItemInfo> infos;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
                    qint32 author_index;
                    qint32 map_version_index;
                    qint32 credits_index;
                    qint32 license_index;
                    qint32 settings_index;
    std::vector<MAPItemImage> images;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
                    qint32 width;
                    qint32 height;
                    qint32 is_external;
                    qint32 image_name_index;
                    qint32 image_data_index;
    std::vector<MAPItemSound> sounds;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
                    qint32 is_external;
                    qint32 sound_name_index;
                    qint32 sound_data_index;
                    qint32 sound_data_size;
    std::vector<MAPItemEnvelope> envelopes;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
                    qint32 channels;
                    qint32 start_point;
                    qint32 points_count;
                    std::array<qint32, map::MAX_ITEM_ENVELOPE_NAME_LENGTH> name;
                    qint32 is_synchronized;
    std::vector<MAPItemEnvelopePoint> envelope_points;
                    \
                    qint32 time;
                    map::CurveType curve_type;
                    std::array<qint32, 4> values;
    std::vector<MAPItemGroup> groups;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
                    qint32 offset_x;
                    qint32 offset_y;
                    qint32 parallax_x;
                    qint32 parallax_y;
                    qint32 start_layer;
                    qint32 layers_count;
                    qint32 has_clipping;
                    qint32 clip_x;
                    qint32 clip_y;
                    qint32 clip_w;
                    qint32 clip_h;
                    std::array<qint32, map::MAX_ITEM_GROUP_NAME_LENGTH> name;
    std::vector<MAPItemLayerInfo> layer_infos;
                    \
                    qint32 type_and_id;
                    qint32 size;
                    qint32 version;
                    map::LayerType type;
                    qint32 flags;
    std::vector<std::variant<MAPItemLayerTile,
                                 \
                                 MAPItemLayerInfo info;
                                 qint32 version;
                                 qint32 width;
                                 qint32 height;
                                 map::SpecialLayerType special_type;
                                 util::Color color;
                                 qint32 color_envelope_index;
                                 qint32 color_envelope_offset;
                                 qint32 image_index;
                                 qint32 data_index;
                                 std::array<qint32, map::MAX_ITEM_LAYER_NAME_LENGTH> name;
                                 qint32 tele_index;
                                 qint32 speedup_index;
                                 qint32 front_index;
                                 qint32 switch_index;
                                 qint32 tune_index;
                             MAPItemLayerQuad, 
                                 \
                                 MAPItemLayerInfo info;
                                 qint32 version;
                                 qint32 quads_count;
                                 qint32 data_index;
                                 qint32 image_index;
                                 std::array<qint32, map::MAX_ITEM_LAYER_NAME_LENGTH> name;
                             MAPItemLayerSound>> layers;
                                 \
                                 MAPItemLayerInfo info;
                                 qint32 version;
                                 qint32 sources_count;
                                 qint32 data_index;
                                 qint32 sound_index;
                                 std::array<qint32, map::MAX_ITEM_LAYER_NAME_LENGTH> name;

*/





/*
                    DATA SPECIFICATION

***********************************************************
*
* Data compressed with zlib.
* 
* Fields:
* header -> file header.
*     Fields:
*     id -> 0x41544144u.
*     version -> 4.
*     size -> file size - 16 bytes.
*     swaplen -> file size - data size - 16 bytes.
*     item_types_count -> number of item types.
*     items_count -> number of items.
*     raw_data_count -> number of data offsets and data 
*                       sizes.
*     item_size -> size of the item data.
*     data_size -> size of the compressed data.
* 
* file_info -> file details.
*     Fields:
*     item_types -> item types.
*         Fields:
*         type -> one of the following:
*             - Version
*             - Info
*             - Image
*             - Envelope
*             - Group
*             - Layer
*             - EnvelopePoints
*             - Sound
*         start -> indicates the byte, where the items are
*                  located.
*         count -> number of items to read.
*     item_offsets -> offsets of the items.
*     data_offsets -> offsets of the compressed data.
*     data_sizes -> sizes of the compressed data.
*     item_data -> item data.
*     compressed_data -> compressed data.
* 
* details -> map details.
*     Fields:
*     author -> the author of the map.
*     map_version -> map version.
*     credits -> map credits.
*     license -> map license.
* 
* assets -> map assets.
*     Fields:
*     images -> image assets.
*         Fields:
*         name -> image name.
*         data -> image data.
          width -> image width.
          height -> image height.
*         is_external -> 1 - external, 0 - embedded.
*     sounds -> sound assets.
*         Fields:
*         name -> sound name.
*         data -> sound data.
*         is_external -> 1 - external, 0 - embedded.
* 
* settings -> server commands.
* envelopes -> envelopes.
*     Fields:
*     points -> envelope points.
*         Fields:
*         time -> time in milliseconds.
*         curve_type -> one of the following:
*             - Step
*             - Linear
*             - Slow
*             - Fast
*             - Smooth
*         values -> envelope channel values.
*     name -> envelope name.
*     channels -> number of all channels.
*     is_synchronized -> 1 - synchronized,
*                        0 - not synchronized.
* 
* groups -> groups.
*     Fields:
*     name -> group name.
*     layers -> one of the following:
*         LayerTile -> tile layer.
*             Fields:
*             name -> layer name.
*             asset_index -> index of the image asset. -1
*                            if no image is used.
*             is_detail -> 1 - detail, 0 - not detail.
*             width -> layer width.
*             height -> layer height.
*             special_type -> one of the following:
*                 - None
*                 - Game
*                 - Tele
*                 - Speedup
*                 - Front
*                 - Switch
*                 - Tune
*             color -> rgba color.
*             color_envelope_index -> index of the color
*                                     envelope. -1 if 
*                                     unused.
*             color_envelope_offset -> time offset in 
*                                      milliseconds.
*             tiles -> layer tiles.
*                 Fields:
*                 index -> tile index.
*                 flags -> 1 - VFlip, 
                           2 - HFlip, 
                           4 - Opaque, 
                           8 - Rotate.
*                 skip -> TODO.
*                 reserved -> reserved.
*             tiles_info -> one of the following:
*                 TeleTileInfo -> tele layer tiles.
*                     Fields:
*                     number -> tele number.
*                     index -> tile index.
*                 SpeedupTileInfo -> speedup layer tiles.
*                     Fields:
*                     force -> speeder force.
*                     max_speed -> maximum speed.
*                     index -> tile index.
*                     angle -> speeder angle.
*                 SwitchTileInfo -> switch layer tiles.
*                     Fields:
*                     number -> switch number.
*                     index -> tile index.
*                     flags -> 1 - VFlip, 
*                              2 - HFlip, 
*                              4 - Opaque,
*                              8 - Rotate.
*                     delay -> switch delay.
*                 TuneTileInfo -> tune layer tiles.
*                     Fields:
*                     number -> tune number.
*                     index -> tile index.
*         LayerQuad -> quad layer.
*             Fields:
*             name -> layer name.
*             asset_index -> index of the image asset. -1
*                            if no image is used.
*             is_detail -> 1 - detail, 0 - not detail.
*             quads -> layer quads.
*                 Fields:
*                 points -> position of the vertices.
*                 colors -> color of the vertices.
*                 tex_coords -> texture coordinates.
*                 position_envelope_index -> index of the
*                                            position
*                                            envelope. -1
*                                            if unused.
*                 position_envelope_offset -> time offset
*                                             in 
*                                             milliseconds.
*                 color_envelope_index -> index of the 
*                                         color envelope. 
*                                         -1 if unused.
*                 color_envelope_offset -> time offset in 
*                                          milliseconds.
*         LayerSound -> sound layer.
*             Fields:
*             name -> layer name.
*             asset_index -> index of the sound asset. -1
*                            if no sound is used.
*             is_detail -> 1 - detail, 0 - not detail.
*             sound_sources -> sound sources.
*                 Fields:
*                 position -> source position.
*                 is_looped -> 1 - looped, 0 - not looped.
*                 has_panning -> 1 - panning, 
*                                0 - no panning.
*                 time_delay_sec -> delay in seconds.
*                 falloff -> 0 to 255.
*                 position_envelope_index -> index of the
*                                            position
*                                            envelope. -1
*                                            if not used.
*                 position_envelope_offset -> time offset
*                                             in 
*                                             milliseconds.
*                 sound_envelope_index -> index of the 
*                                         sound envelope. 
*                                         -1 if unused.
*                 sound_envelope_offset -> time offset in 
*                                          milliseconds.
*                 shape -> source shape.
*                     Fields:
*                     type -> one of the following:
*                         - Rectangle
*                         - Circle
*                     size -> either rectangle size or 
*                             circle radius.
*     offset_x -> horizontal offset for group layers.
*     offset_y -> vertical offset for group layers.
*     parallax_x -> horizontal parallax for group layers.
*     parallax_y -> vertical parallax for group layers.
*     clip_x -> position x of the clipping rectangle.
*     clip_y -> position y of the clipping rectangle.
*     clip_w -> width of the clipping rectangle.
*     clip_h -> height of the clipping rectangle.
*     has_clipping -> 1 - clipping, 0 - no clipping.
*
***********************************************************

MapData
    \
    MAPHeader header;
        \
        qint32 id;
        qint32 version;
        qint32 size;
        qint32 swaplen;
        qint32 item_types_count;
        qint32 items_count;
        qint32 raw_data_count;
        qint32 item_size;
        qint32 data_size;
    MAPFileInfo file_info;
        \
        std::vector<MAPItemType> item_types;
                        \
                        map::ItemType type;
                        qint32 start;
                        qint32 count;
        std::vector<qint32> item_offsets;
        std::vector<qint32> data_offsets;
        std::vector<qint32> data_sizes;
        QByteArray item_data;
        QByteArray compressed_data;
        qint32 item_offsets_end;
        qint32 data_offsets_end;
    MAPDetails details;
        \
        QByteArray author;
        QByteArray map_version;
        QByteArray credits;
        QByteArray license;
    MAPAssets assets;
        \
        std::vector<ImageAsset> images;
                        \
                        QByteArray name;
                        QByteArray data;
                        qint32 width;
                        qint32 height;
                        bool is_external;
        std::vector<SoundAsset> sounds;
                        \
                        QByteArray name;
                        QByteArray data;
                        bool is_external;
    std::vector<QByteArray> settings;
    std::vector<Envelope> envelopes;
                    \
                    std::vector<EnvelopePoint> points;
                                    \
                                    qint32 time;
                                    map::CurveType curve_type;
                                    std::array<qint32, 4> values;
                    QByteArray name;
                    qint32 channels;
                    bool is_synchronized;
    std::vector<Group> groups;
                    \
                    QByteArray name;
                    std::vector<std::variant<LayerTile, 
                                                 \
                                                 QByteArray name;
                                                 qint32 asset_index;
                                                 bool is_detail;
                                                 qint32 width;
                                                 qint32 height;
                                                 map::SpecialLayerType special_type;
                                                 util::Color color;
                                                 qint32 color_envelope_index;
                                                 qint32 color_envelope_offset;
                                                 std::vector<Tile> tiles;
                                                                 \
                                                                 quint8 index;
                                                                 quint8 flags;
                                                                 quint8 skip;
                                                                 quint8 reserved;
                                                 std::vector<std::variant<TeleTileInfo, 
                                                                              \
                                                                              quint8 number;
                                                                              quint8 index;
                                                                          SpeedupTileInfo, 
                                                                              \
                                                                              quint8 force;
                                                                              quint8 max_speed;
                                                                              quint8 index;
                                                                              qint16 angle;
                                                                          SwitchTileInfo, 
                                                                              \
                                                                              quint8 number;
                                                                              quint8 index;
                                                                              quint8 flags;
                                                                              quint8 delay;
                                                                          TuneTileInfo>> tiles_info;
                                                                              \
                                                                              quint8 number;
                                                                              quint8 index;
                                             LayerQuad, 
                                                 \
                                                 QByteArray name;
                                                 qint32 asset_index;
                                                 bool is_detail;
                                                 std::vector<Quad> quads;
                                                                 \
                                                                 std::array<util::Point, 5> points;
                                                                 std::array<util::Color, 4> colors;
                                                                 std::array<util::Point, 4> tex_coords;
                                                                 qint32 position_envelope_index;
                                                                 qint32 position_envelope_offset;
                                                                 qint32 color_envelope_index;
                                                                 qint32 color_envelope_offset;
                                             LayerSound>> layers;
                                                 \
                                                 QByteArray name;
                                                 qint32 asset_index;
                                                 bool is_detail;
                                                 std::vector<SoundSource> sound_sources;
                                                                 \
                                                                 util::Point position;
                                                                 qint32 is_looped;
                                                                 qint32 has_panning;
                                                                 qint32 time_delay_sec;
                                                                 qint32 falloff;
                                                                 qint32 position_envelope_index;
                                                                 qint32 position_envelope_offset;
                                                                 qint32 sound_envelope_index;
                                                                 qint32 sound_envelope_offset;
                                                                 SoundShape shape;
                                                                     \
                                                                     map::SoundSourceShapeType type;
                                                                     std::variant<util::RectangleSize, util::CircleRadius> size;
                    qint32 offset_x;
                    qint32 offset_y;
                    qint32 parallax_x;
                    qint32 parallax_y;
                    qint32 clip_x;
                    qint32 clip_y;
                    qint32 clip_w;
                    qint32 clip_h;
                    bool has_clipping;

*/

} // ddnet::io::
