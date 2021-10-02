#pragma once

#include <ddnet/util/enums.hpp>
#include <ddnet/util/utility.hpp>

#include <QtGlobal>

namespace ddnet {

struct Tile {
    quint8 index;
    enums::Rotation rotation;
    bool is_selected;
};



struct TeleTile : Tile {
    quint8 id;
};



struct TuneTile : Tile {
    quint8 id;
};



struct SpeedupTile : Tile {
    quint8 force;
    quint8 max_speed;
    qint16 angle;
};



struct SwitchTile : Tile {
    quint8 id;
    quint8 delay;
};

}
