#pragma once

#include <vt/util/constants.hpp>

#include <QPoint>

#include <array>

namespace vt::core {

template <typename _TileT>
class Chunk {
public:
    inline Chunk() = delete;

    inline Chunk(quint16 x, quint16 y)
        : id{ static_cast<quint32>(y * constants::_grid_size + x) }
    {
    }

    inline auto& tileAt(quint16 x, quint16 y) { return tiles[y * constants::_chunk_size + x]; }
    inline const auto& tileAt(quint16 x, quint16 y) const { return tiles[y * constants::_chunk_size + x]; }
    inline auto getID() { return id; }

public:
    std::array<_TileT, constants::_chunk_size * constants::_chunk_size> tiles{};

private:
    quint32 id;
};

}
