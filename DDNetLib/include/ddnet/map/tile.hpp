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



class TeleTile {
public:
    inline TeleTile(const TeleTile&) = default;
    inline TeleTile(TeleTile&&) = default;
    inline TeleTile& operator=(const TeleTile&) = default;
    inline TeleTile& operator=(TeleTile&&) = default;

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
    inline TuneTile(const TuneTile&) = default;
    inline TuneTile(TuneTile&&) = default;
    inline TuneTile& operator=(const TuneTile&) = default;
    inline TuneTile& operator=(TuneTile&&) = default;

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
    inline SpeedupTile(const SpeedupTile&) = default;
    inline SpeedupTile(SpeedupTile&&) = default;
    inline SpeedupTile& operator=(const SpeedupTile&) = default;
    inline SpeedupTile& operator=(SpeedupTile&&) = default;

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
    inline SwitchTile(const SwitchTile&) = default;
    inline SwitchTile(SwitchTile&&) = default;
    inline SwitchTile& operator=(const SwitchTile&) = default;
    inline SwitchTile& operator=(SwitchTile&&) = default;

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
