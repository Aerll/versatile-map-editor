#pragma once

#include <QtGlobal>

#include <array>

namespace vt::core {

template <typename _TileT, qsizetype _SideLength = 128>
class Chunk {
    inline Chunk(quint16 id)
        : tiles{}
        , id{ id }
    {
    }

    inline quint16 getID() { return id; }

public:
    std::array<_TileT, _SideLength * _SideLength> tiles;

protected:
    quint16 id;
};

}
