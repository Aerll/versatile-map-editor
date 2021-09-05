#pragma once

#include <ddnet/util/macros.hpp>

#include <QtGlobal>

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

struct Tile {
    quint8 index;
    Rotation rotation;
    bool is_opaque;
};



struct TeleTile {
    quint8 index;
    quint8 id;
};



struct TuneTile {
    quint8 index;
    quint8 id;
};



struct SpeedupTile {
    quint8 index;
    quint8 force;
    quint8 max_speed;
    qint16 angle;
};



struct SwitchTile {
    quint8 index;
    quint8 id;
    Rotation rotation;
    quint8 delay;
};

} // ddnet::map::
