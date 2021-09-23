#pragma once

#include <ddnet/map/tile.hpp>

#include <QtGlobal>

#include <array>

namespace ddnet::map {

class Node {
    inline Node(quint16 id)
        : tiles{}
        , id{ id }
    {
    }

    inline quint16 getID() { return id; }

public:
    std::array<Tile, 256 * 256> tiles;

protected:
    quint16 id;
};

} // ddnet::map::
