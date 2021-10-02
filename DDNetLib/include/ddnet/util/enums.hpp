#pragma once

#include <ddnet/util/utility.hpp>

#include <QtGlobal>

namespace ddnet::enums {

enum class ItemType : qint32 {
    Version = 0,
    Info,
    Image,
    Envelope,
    Group,
    Layer,
    EnvelopePoints,
    Sound,
};

enum class LayerType : qint32 {
    Invalid = 0,
    Tile    = 2,
    Quad    = 3,
    Sound   = 10,
};

enum class SpecialLayerType : qint32 {
    None    = 0,
    Game    = util::flag(0),
    Tele    = util::flag(1),
    Speedup = util::flag(2),
    Front   = util::flag(3),
    Switch  = util::flag(4),
    Tune    = util::flag(5),
};

enum class CurveType : qint32 {
    Step = 0,
    Linear,
    Slow,
    Fast,
    Smooth,
};

enum class SoundSourceShapeType : qint32 {
    Rectangle = 0,
    Circle
};



constexpr qint32 operator<<(enums::ItemType item_type, qint32 value) noexcept {
    return util::toUnderlying(item_type) << value;
}

} // ddnet::enums::
