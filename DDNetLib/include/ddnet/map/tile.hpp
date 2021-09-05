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

class Tile {
public:
    DEFAULT_COPY_(Tile);
    DEFAULT_MOVE_(Tile);

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



class TeleTile {
public:
    DEFAULT_COPY_(TeleTile);
    DEFAULT_MOVE_(TeleTile);

    inline TeleTile()
        : index{ 0 }
        , id{ 0 }
    {
    }

    inline TeleTile(quint8 index, quint8 id)
        : index{ index }
        , id{ id }
    {
    }

public:
    quint8 index;
    quint8 id;
};



class TuneTile {
public:
    DEFAULT_COPY_(TuneTile);
    DEFAULT_MOVE_(TuneTile);

    inline TuneTile()
        : index{ 0 }
        , id{ 0 }
    {
    }

    inline TuneTile(quint8 index, quint8 id)
        : index{ index }
        , id{ id }
    {
    }

public:
    quint8 index;
    quint8 id;
};



class SpeedupTile {
public:
    DEFAULT_COPY_(SpeedupTile);
    DEFAULT_MOVE_(SpeedupTile);

    inline SpeedupTile()
        : index{ 0 }
        , force{ 0 }
        , max_speed{ 0 }
        , angle{ 0 }
    {
    }

    inline SpeedupTile(quint8 index, quint8 force, quint8 max_speed, qint16 angle)
        : index{ index }
        , force{ force }
        , max_speed{ max_speed }
        , angle{ angle }
    {
    }

public:
    quint8 index;
    quint8 force;
    quint8 max_speed;
    qint16 angle;
};



class SwitchTile {
public:
    DEFAULT_COPY_(SwitchTile);
    DEFAULT_MOVE_(SwitchTile);

    inline SwitchTile()
        : index{ 0 }
        , id{ 0 }
        , rotation{ Rotation::N }
        , delay{ 0 }
    {
    }

    inline SwitchTile(quint8 index, quint8 id, Rotation rotation, quint8 delay)
        : index{ index }
        , id{ id }
        , rotation{ rotation }
        , delay{ delay }
    {
    }

public:
    quint8 index;
    quint8 id;
    Rotation rotation;
    quint8 delay;
};

} // ddnet::map::
