#pragma once

#include <QtGlobal>

namespace ddnet::constants {

constexpr qint32 _notfound = -1;
constexpr qint32 _fractional_bits = 10;

} // ddnet::constants::

namespace ddnet::map {

constexpr qint32 _id = 0x41544144u; // DATA
constexpr qint32 _version = 4;
constexpr qint32 _item_version = 1;
constexpr qint32 _item_envelope_version = 2;
constexpr qint32 _item_group_version = 3;
constexpr qint32 _item_layer_tile_version = 3;
constexpr qint32 _item_layer_quad_version = 2;
constexpr qint32 _item_layer_sound_version = 2;

enum {
    MAX_AUTHOR_LENGTH             = 32,
    MAX_MAP_VERSION_LENGTH        = 16,
    MAX_CREDITS_LENGTH            = 128,
    MAX_LICENSE_LENGTH            = 32,

    MAX_COMMAND_LENGTH            = 256,

    MAX_ITEM_ENVELOPE_NAME_LENGTH = 8,
    MAX_ITEM_GROUP_NAME_LENGTH    = 3,
    MAX_ITEM_LAYER_NAME_LENGTH    = 3,
    MAX_ENVELOPE_NAME_LENGTH      = MAX_ITEM_ENVELOPE_NAME_LENGTH * sizeof(qint32),
    MAX_GROUP_NAME_LENGTH         = MAX_ITEM_GROUP_NAME_LENGTH * sizeof(qint32),
    MAX_LAYER_NAME_LENGTH         = MAX_ITEM_LAYER_NAME_LENGTH * sizeof(qint32),
};

enum class ItemType {
    Version = 0,
    Info,
    Image,
    Envelope,
    Group,
    Layer,
    EnvelopePoints,
    Sound,
};

enum class LayerType {
    Invalid = 0,
    Tile    = 2,
    Quad    = 3,
    Sound   = 10,
};

enum class SpecialLayerType {
    None    = 0,
    Game    = 1 << 0,
    Tele    = 1 << 1,
    Speedup = 1 << 2,
    Front   = 1 << 3,
    Switch  = 1 << 4,
    Tune    = 1 << 5,
};

enum class CurveType {
    Step = 0,
    Linear,
    Slow,
    Fast,
    Smooth,
};

enum class SoundSourceShapeType {
    Rectangle = 0,
    Circle
};



inline constexpr qint32 operator<<(ItemType item_type, qint32 value) noexcept {
    return static_cast<qint32>(item_type) << value;
}

} // ddnet::map::
