#pragma once

#include <ddnet/util/macros.hpp>
#include <ddnet/util/utility.hpp>

#include <QtGlobal>

namespace ddnet::enums {

enum class Rotation : quint8 {
    N = 0,
    V = util::flag(0),
    H = util::flag(1),
    R = util::flag(3),

    VH  = V | H,
    VR  = V | R,
    HR  = H | R,
    VHR = V | H | R,
};
ENUM_DEFINE_BITWISE_OPS_(Rotation);



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
constexpr qint32 operator<<(enums::ItemType item_type, qint32 value) noexcept {
    return util::toUnderlying(item_type) << value;
}



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



enum class TokenType {
    Invalid    = util::flag(0),
    Identifier = util::flag(1),
    Literal    = util::flag(2),
    Keyword    = util::flag(3),
    Operator   = util::flag(4),
    Open       = util::flag(5),
    Close      = util::flag(6),
    Terminal   = util::flag(7),
};
ENUM_DEFINE_AND_(TokenType);
ENUM_DEFINE_OR_(TokenType);



enum class Rotate {
    Clockwise,
    CounterClockwise,
};



enum class Mirror {
    VerticalAxis,
    HorizontalAxis,
};

}
