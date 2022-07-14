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

    [[nodiscard]] inline auto getID() const noexcept { return id; }

    [[nodiscard]] inline auto& tileAt(qint32 x, qint32 y) { return tiles[y * constants::_chunk_size + x]; }
    [[nodiscard]] inline auto& tileAt(QPoint pos) { return tileAt(pos.x(), pos.y()); }
    [[nodiscard]] inline const auto& tileAt(qint32 x, qint32 y) const { return tiles[y * constants::_chunk_size + x]; }
    [[nodiscard]] inline const auto& tileAt(QPoint pos) const { return tileAt(pos.x(), pos.y()); }

    inline void replaceAt(qint32 x, qint32 y, const _TileT& tile) { tileAt(x, y) = tile; }
    inline void replaceAt(QPoint pos, const _TileT& tile) { replaceAt(pos.x(), pos.y(), tile); }

    void replaceArea(qint32 x, qint32 y, qint32 width, qint32 height, const _TileT& tile) {
        assert((QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }.contains(QRect{ x, y, width, height })));
        for (qint32 pos_x = x; pos_x < x + width; ++pos_x) {
            for (qint32 pos_y = y; pos_y < y + height; ++pos_y) {
                tileAt(pos_x, pos_y) = tile;
            }
        }
    }
    inline void replaceArea(QPoint pos, QSize size, const _TileT& tile) { replaceArea(pos.x(), pos.y(), size.width(), size.height(), tile); }
    inline void replaceArea(QRect rect, const _TileT& tile) { replaceArea(rect.x(), rect.y(), rect.width(), rect.height(), tile); }

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
    inline void clearArea(QPoint pos, QSize size) { clearArea(pos.x(), pos.y(), size.width(), size.height()); }
    inline void clearArea(QRect rect) { clearArea(rect.x(), rect.y(), rect.width(), rect.height()); }

    [[nodiscard]] auto findAll(qint32 x, qint32 y, qint32 width, qint32 height, const _TileT& tile) const {
        assert((QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }.contains(QRect{ x, y, width, height })));
        std::vector<QPoint> matched_tiles;

        quint32 count = 0;
        for (qint32 pos_x = x; pos_x < x + width; ++pos_x) {
            for (qint32 pos_y = y; pos_y < y + height; ++pos_y)
                count += tileAt(pos_x, pos_y) == tile;
        }
        matched_tiles.reserve(count);

        for (qint32 pos_x = x; pos_x < x + width; ++pos_x) {
            for (qint32 pos_y = y; pos_y < y + height; ++pos_y)
                if (tileAt(pos_x, pos_y) == tile)
                    matched_tiles.emplace_back(pos_x, pos_y);
        }
        return matched_tiles;
    }
    [[nodiscard]] inline auto findAll(QPoint pos, QSize size, const _TileT& tile) const { return findAll(pos.x(), pos.y(), size.width(), size.height(), tile); }
    [[nodiscard]] inline auto findAll(QRect rect, const _TileT& tile) const { return findAll(rect.x(), rect.y(), rect.width(), rect.height(), tile); }
    [[nodiscard]] inline auto findAll(const _TileT& tile) const { return findAll(0, 0, constants::_chunk_size, constants::_chunk_size, tile); }

public:
    std::array<_TileT, constants::_chunk_size * constants::_chunk_size> tiles{};

private:
    quint32 id;
};

}
