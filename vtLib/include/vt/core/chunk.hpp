#pragma once

#include <vt/util/constants.hpp>

#include <QRect>

#include <array>
#include <cassert>

namespace vt::core {

template <typename _TileT>
class Chunk {
public:
    inline Chunk() = delete;

    inline Chunk(qint32 x, qint32 y)
        : id{ y * constants::_grid_size + x }
    {
    }
    inline Chunk(QPoint pos)
        : Chunk{ pos.x(), pos.y() }
    {
    }

    inline auto& tileAt(qint32 x, qint32 y) { return tiles[y * constants::_chunk_size + x]; }
    inline auto& tileAt(QPoint pos) { return tileAt(pos.x(), pos.y()); }
    inline const auto& tileAt(qint32 x, qint32 y) const { return tiles[y * constants::_chunk_size + x]; }
    inline const auto& tileAt(QPoint pos) const { return tileAt(pos.x(), pos.y()); }
    inline auto getID() { return id; }

    inline void clearAt(qint32 x, qint32 y) { tiles[y * constants::_chunk_size + x] = {}; }
    inline void clearAt(QPoint pos) { clearAt(pos.x(), pos.y()); }
    
    void clearArea(qint32 x, qint32 y, qint32 width, qint32 height) {
        assert((QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }.contains(QRect{ x, y, width, height })));
        for (qint32 pos_x = x; pos_x < x + width; ++pos_x) {
            for (qint32 pos_y = y; pos_y < y + height; ++pos_y) {
                tileAt(pos_x, pos_y) = {};
            }
        }
    }
    inline void clearArea(QPoint pos, QSize size) {
        clearArea(pos.x(), pos.y(), size.width(), size.height());
    }
    inline void clearArea(QRect rect) {
        clearArea(rect.x(), rect.y(), rect.width(), rect.height());
    }

public:
    std::array<_TileT, constants::_chunk_size * constants::_chunk_size> tiles{};

private:
    quint32 id;
};

}
