#pragma once

#include <vt/map/tile.hpp>

#include <QtGlobal>

#include <array>

namespace vt {

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

}
