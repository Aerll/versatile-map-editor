#pragma once

#include <vt/util/constants.hpp>
#include <vt/util/enums.hpp>
#include <vt/util/types.hpp>
#include <vt/util/utility.hpp>

#include <QByteArray>

#include <array>
#include <vector>
#include <variant>
#include <algorithm>
#include <iterator>

namespace vt::io {

struct MAPHeader;
struct MAPFile;
struct MAPFileInfo;
struct MAPItemType;
struct MAPDataFileItem;
struct MAPItemVersion;
struct MAPItemInfo;
struct MAPItemImage;
struct MAPItemSound;
struct MAPItemEnvelope;
struct MAPItemEnvelopePoint;
struct MAPItemGroup;
struct MAPItemLayerInfo;
struct MAPItemLayerTile;
struct MAPItemLayerQuad;
struct MAPItemLayerSound;
struct MAPDetails;
struct MAPAssets;

struct MAPEnvelope;
struct MAPEnvelopePoint;
struct MAPGroup;
struct MAPLayer;
struct MAPLayerTile;
struct MAPLayerQuad;
struct MAPLayerSound;
struct MAPTile;
struct MAPTeleTileInfo;
struct MAPSpeedupTileInfo;
struct MAPSwitchTileInfo;
struct MAPTuneTileInfo;
struct MAPQuad;
struct MAPSoundShape;
struct MAPSoundSource;
struct MAPImageAsset;
struct MAPSoundAsset;

struct MAPItemData;
class MAPData;



struct MAPHeader {
    qint32 id;
    qint32 version;
    qint32 size;
    qint32 swaplen;
    qint32 item_types_count;
    qint32 items_count;
    qint32 raw_data_count;
    qint32 item_size;
    qint32 data_size;
};



struct MAPFileInfo {
    std::vector<MAPItemType> item_types;
    std::vector<qint32> item_offsets;
    std::vector<qint32> data_offsets;
    std::vector<qint32> data_sizes;
    QByteArray item_data;
    QByteArray compressed_data;
    qint32 item_offsets_end;
    qint32 data_offsets_end;
};



struct MAPItemType {
    enums::ItemType type;
    qint32 start;
    qint32 count;
};



struct MAPDataFileItem {
    qint32 type_and_id;
    qint32 size;
};



struct MAPItemVersion : MAPDataFileItem {
    qint32 version;
};



struct MAPItemInfo : MAPDataFileItem {
    qint32 version;
    qint32 author_index;
    qint32 map_version_index;
    qint32 credits_index;
    qint32 license_index;
    qint32 settings_index;
};



struct MAPItemImage : MAPDataFileItem {
    qint32 version;
    qint32 width;
    qint32 height;
    qint32 is_external;
    qint32 image_name_index;
    qint32 image_data_index;
};



struct MAPItemSound : MAPDataFileItem {
    qint32 version;
    qint32 is_external;
    qint32 sound_name_index;
    qint32 sound_data_index;
    qint32 sound_data_size;
};



struct MAPItemEnvelope : MAPDataFileItem {
    qint32 version;
    qint32 channels;
    qint32 start_point;
    qint32 points_count;
    std::array<qint32, constants::_map_max_item_envelope_name_length> name;
    qint32 is_synchronized;
};



struct MAPItemEnvelopePoint {
    qint32 time;
    enums::CurveType curve_type;
    std::array<qint32, 4> values;
};



struct MAPItemGroup : MAPDataFileItem {
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
    std::array<qint32, constants::_map_max_group_name_length> name;
};



struct MAPItemLayerInfo : MAPDataFileItem {
    qint32 version;
    enums::LayerType type;
    qint32 is_detail;
};



struct MAPItemLayerTile {
    MAPItemLayerInfo info;
    qint32 version;
    qint32 width;
    qint32 height;
    enums::SpecialLayerType special_type;

    util::Color color;
    qint32 color_envelope_index;
    qint32 color_envelope_offset;

    qint32 image_index;
    qint32 data_index;
    std::array<qint32, constants::_map_max_item_layer_name_length> name;

    qint32 tele_index;
    qint32 speedup_index;
    qint32 front_index;
    qint32 switch_index;
    qint32 tune_index;
};



struct MAPItemLayerQuad {
    MAPItemLayerInfo info;
    qint32 version;
    qint32 quads_count;
    qint32 data_index;
    qint32 image_index;
    std::array<qint32, constants::_map_max_item_layer_name_length> name;
};



struct MAPItemLayerSound {
    MAPItemLayerInfo info;
    qint32 version;
    qint32 sources_count;
    qint32 data_index;
    qint32 sound_index;
    std::array<qint32, constants::_map_max_item_layer_name_length> name;
};



struct MAPDetails {
    QByteArray author;
    QByteArray map_version;
    QByteArray credits;
    QByteArray license;
};



struct MAPAssets {
    std::vector<MAPImageAsset> images;
    std::vector<MAPSoundAsset> sounds;
};



struct MAPEnvelope {
    std::vector<MAPEnvelopePoint> points;
    QByteArray name;
    qint32 channels;
    bool is_synchronized;
};



struct MAPEnvelopePoint {
    qint32 time;
    enums::CurveType curve_type;
    std::array<qint32, 4> values;
};



struct MAPGroup {
    QByteArray name;
    std::vector<std::variant<MAPLayerTile, MAPLayerQuad, MAPLayerSound>> layers;
    qint32 offset_x;
    qint32 offset_y;
    qint32 parallax_x;
    qint32 parallax_y;
    qint32 clip_x;
    qint32 clip_y;
    qint32 clip_w;
    qint32 clip_h;
    bool has_clipping;
};



struct MAPLayer {
    QByteArray name;
    qint32 asset_index;
    bool is_detail;
};



struct MAPLayerTile : MAPLayer {
    qint32 width;
    qint32 height;
    enums::SpecialLayerType special_type;

    util::Color color;
    qint32 color_envelope_index;
    qint32 color_envelope_offset;

    std::vector<MAPTile> tiles;
    std::vector<std::variant<MAPTeleTileInfo, MAPSpeedupTileInfo, MAPSwitchTileInfo, MAPTuneTileInfo>> tiles_info;
};



struct MAPLayerQuad : MAPLayer {
    std::vector<MAPQuad> quads;
};



struct MAPLayerSound : MAPLayer {
    std::vector<MAPSoundSource> sound_sources;
};



struct MAPTile {
    quint8 index;
    quint8 flags;
    quint8 skip;
    quint8 reserved;
};



struct MAPTeleTileInfo {
    quint8 number;
    quint8 index;
};



struct MAPSpeedupTileInfo {
    quint8 force;
    quint8 max_speed;
    quint8 index;
    qint16 angle;
};



struct MAPSwitchTileInfo {
    quint8 number;
    quint8 index;
    quint8 flags;
    quint8 delay;
};



struct MAPTuneTileInfo {
    quint8 number;
    quint8 index;
};



struct MAPQuad {
    std::array<util::Point, 5> points;
    std::array<util::Color, 4> colors;
    std::array<util::Point, 4> tex_coords;

    qint32 position_envelope_index;
    qint32 position_envelope_offset;

    qint32 color_envelope_index;
    qint32 color_envelope_offset;
};



struct MAPSoundShape {
    enums::SoundSourceShapeType type;
    std::variant<util::RectangleSize, util::CircleRadius> size;
};



struct MAPSoundSource {
    util::Point position;
    qint32 is_looped;
    qint32 has_panning;
    qint32 time_delay_sec;
    qint32 falloff; // from 0 to 255

    qint32 position_envelope_index;
    qint32 position_envelope_offset;

    qint32 sound_envelope_index;
    qint32 sound_envelope_offset;

    MAPSoundShape shape;
};



struct MAPImageAsset {
    QByteArray name;
    QByteArray data;
    qint32 width;
    qint32 height;
    bool is_external;
};



struct MAPSoundAsset {
    QByteArray name;
    QByteArray data;
    bool is_external;
};



struct MAPItemData {
    MAPItemType type;
    std::vector<MAPItemVersion> versions;
    std::vector<MAPItemInfo> infos;
    std::vector<MAPItemImage> images;
    std::vector<MAPItemSound> sounds;
    std::vector<MAPItemEnvelope> envelopes;
    std::vector<MAPItemEnvelopePoint> envelope_points;
    std::vector<MAPItemGroup> groups;
    std::vector<MAPItemLayerInfo> layer_infos;
    std::vector<std::variant<MAPItemLayerTile, MAPItemLayerQuad, MAPItemLayerSound>> layers;
};



class DDMData;

class MAPData {
private:
    template <typename _Type>
    inline qint32 layersCountImpl() const {
        qint32 result = 0;
        for (const auto& group : groups) {
            result += std::ranges::count_if(group.layers, [](const auto& layer) {
                return layer.index() == util::typeIndex<_Type>(layer);
            });
        }
        return result;
    }

public:
    inline const auto& author() const noexcept { return details.author; }
    inline const auto& mapVersion() const noexcept { return details.map_version; }
    inline const auto& credits() const noexcept { return details.credits; }
    inline const auto& license() const noexcept { return details.license; }

    inline const auto& imageAssets() const noexcept { return assets.images; }
    inline const auto& soundAssets() const noexcept { return assets.sounds; }

    inline qint32 tileLayersCount() const { return layersCountImpl<MAPLayerTile>(); }
    inline qint32 quadLayersCount() const { return layersCountImpl<MAPLayerQuad>(); }
    inline qint32 soundLayersCount() const { return layersCountImpl<MAPLayerSound>(); }

    DDMData toDDM() const;

public:
    MAPHeader header;
    MAPFileInfo file_info;
    MAPDetails details;
    MAPAssets assets;
    std::vector<QByteArray> settings;
    std::vector<MAPEnvelope> envelopes;
    std::vector<MAPGroup> groups;
};

}
