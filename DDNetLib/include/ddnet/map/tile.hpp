#pragma once

#include <ddnet/util/macros.hpp>

#include <QtGlobal>
#include <QPoint>

namespace ddnet {

enum class Rotation : quint8 {
    N = 0,
    V = 1 << 0,
    H = 1 << 1,
    R = 1 << 2,

    VH = V | H,
    VR = V | R,
    HR = H | R,
    VHR = V | H | R,
};
ENUM_DEFINE_ALL_(Rotation);

} // ddnet::



namespace ddnet::map {

class Tile {
public:
    inline Tile(const Tile&) = default;
    inline Tile(Tile&&) = default;
    inline Tile& operator=(const Tile&) = default;
    inline Tile& operator=(Tile&&) = default;

    inline Tile()
        : index{ 0 }
        , rotation{ Rotation::N }
        , is_opaque{ false }
    {
    }

    inline Tile(quint8 index, Rotation rotation, bool is_opaque)
        : index{ index }
        , rotation{ rotation }
        , is_opaque{ is_opaque }
    {
    }

public:
    quint8 index;
    Rotation rotation;
    bool is_opaque;
};

} // ddnet::map::
