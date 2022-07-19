#pragma once

#include <vt/core/tile.hpp>
#include <vt/util/constants.hpp>
#include <vt/util/concepts.hpp>

#include <QRect>

#include <array>
#include <cassert>

namespace vt::core {

template <typename _TileT>
class Chunk {
public:
    static inline auto _Clear = [](_TileT& tile) {
        tile = {};
    };
    static inline auto _Select = [](_TileT& tile) {
        tile.select();
    };
    static inline auto _Unselect = [](_TileT& tile) {
        tile.unselect();
    };
    static inline auto _RotateClockwise = [](_TileT& tile) {
        tile.rotate(enums::Rotate::Clockwise);
    };
    static inline auto _RotateCounterClockwise = [](_TileT& tile) {
        tile.rotate(enums::Rotate::CounterClockwise);
    };
    static inline auto _MirrorVerticalAxis = [](_TileT& tile) {
        tile.mirror(enums::Mirror::VerticalAxis);
    };
    static inline auto _MirrorHorizontalAxis = [](_TileT& tile) {
        tile.mirror(enums::Mirror::HorizontalAxis);
    };

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

    [[nodiscard]] inline quint32 getID() const noexcept { return id; }

    [[nodiscard]] inline _TileT& tileAt(qint32 x, qint32 y) { return tiles[y * constants::_chunk_size + x]; }
    [[nodiscard]] inline _TileT& tileAt(QPoint pos) { return tileAt(pos.x(), pos.y()); }
    [[nodiscard]] inline const _TileT& tileAt(qint32 x, qint32 y) const { return tiles[y * constants::_chunk_size + x]; }
    [[nodiscard]] inline const _TileT& tileAt(QPoint pos) const { return tileAt(pos.x(), pos.y()); }

    template <typename _UnaryModifierT> 
    requires (concepts::Invocable<_UnaryModifierT, _TileT&>)
    inline void modifyAt(qint32 x, qint32 y, _UnaryModifierT modifier) {
        assert(x < constants::_chunk_size && y < constants::_chunk_size);
        modifier(tileAt(x, y));
    }
    template <typename _UnaryModifierT> 
    requires (concepts::Invocable<_UnaryModifierT, _TileT&>)
    inline void modifyAt(QPoint pos, _UnaryModifierT modifier) {
        modifyAt(pos.x(), pos.y(), modifier);
    }

    template <typename _UnaryModifierT, typename _UnaryPredicateT> 
    requires (
        concepts::Invocable<_UnaryModifierT, _TileT&> && 
        concepts::Invocable<_UnaryPredicateT, const _TileT&> &&
        requires (_UnaryPredicateT predicate, const _TileT& tile) {
            { predicate(tile) } -> concepts::ConvertibleTo<bool>;
        })
    inline void modifyAtIf(qint32 x, qint32 y, _UnaryModifierT modifier, _UnaryPredicateT predicate) {
        assert(x < constants::_chunk_size && y < constants::_chunk_size);
        if (predicate(tileAt(x, y)))
            modifyAt(x, y, modifier);
    }
    template <typename _UnaryModifierT, typename _UnaryPredicateT> 
    requires (
        concepts::Invocable<_UnaryModifierT, _TileT&> && 
        concepts::Invocable<_UnaryPredicateT, const _TileT&> &&
        requires (_UnaryPredicateT predicate, const _TileT& tile) {
            { predicate(tile) } -> concepts::ConvertibleTo<bool>;
        })
    inline void modifyAtIf(QPoint pos, _UnaryModifierT modifier, _UnaryPredicateT predicate) {
        modifyAtIf(pos.x(), pos.y(), modifier, predicate);
    }

    template <typename _UnaryModifierT> 
    requires (concepts::Invocable<_UnaryModifierT, _TileT&>)
    inline void modifyArea(qint32 x, qint32 y, qint32 width, qint32 height, _UnaryModifierT modifier) {
        assert((QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }.contains(QRect{ x, y, width, height })));
        for (qint32 pos_x = x; pos_x < x + width; ++pos_x) {
            for (qint32 pos_y = y; pos_y < y + height; ++pos_y) {
                modifyAt(pos_x, pos_y, modifier);
            }
        }
    }
    template <typename _UnaryModifierT> 
    requires (concepts::Invocable<_UnaryModifierT, _TileT&>)
    inline void modifyArea(QPoint pos, QSize size, _UnaryModifierT modifier) {
        modifyArea(pos.x(), pos.y(), size.width(), size.height(), modifier);
    }
    template <typename _UnaryModifierT> 
    requires (concepts::Invocable<_UnaryModifierT, _TileT&>)
    inline void modifyArea(QRect rect, _UnaryModifierT modifier) {
        modifyArea(rect.topLeft(), rect.size(), modifier);
    }

    template <typename _UnaryModifierT, typename _UnaryPredicateT> 
    requires (
        concepts::Invocable<_UnaryModifierT, _TileT&> && 
        concepts::Invocable<_UnaryPredicateT, const _TileT&> &&
        requires (_UnaryPredicateT predicate, const _TileT& tile) {
            { predicate(tile) } -> concepts::ConvertibleTo<bool>;
        })
    inline void modifyAreaIf(qint32 x, qint32 y, qint32 width, qint32 height, _UnaryModifierT modifier, _UnaryPredicateT predicate) {
        assert((QRect{ 0, 0, constants::_chunk_size, constants::_chunk_size }.contains(QRect{ x, y, width, height })));
        for (qint32 pos_x = x; pos_x < x + width; ++pos_x) {
            for (qint32 pos_y = y; pos_y < y + height; ++pos_y) {
                modifyAtIf(pos_x, pos_y, modifier, predicate);
            }
        }
    }
    template <typename _UnaryModifierT, typename _UnaryPredicateT> 
    requires (
        concepts::Invocable<_UnaryModifierT, _TileT&> && 
        concepts::Invocable<_UnaryPredicateT, const _TileT&> &&
        requires (_UnaryPredicateT predicate, const _TileT& tile) {
            { predicate(tile) } -> concepts::ConvertibleTo<bool>;
        })
    inline void modifyAreaIf(QPoint pos, QSize size, _UnaryModifierT modifier, _UnaryPredicateT predicate) {
        modifyAreaIf(pos.x(), pos.y(), size.width(), size.height(), modifier, predicate);
    }
    template <typename _UnaryModifierT, typename _UnaryPredicateT> 
    requires (
        concepts::Invocable<_UnaryModifierT, _TileT&> && 
        concepts::Invocable<_UnaryPredicateT, const _TileT&> &&
        requires (_UnaryPredicateT predicate, const _TileT& tile) {
            { predicate(tile) } -> concepts::ConvertibleTo<bool>;
        })
    inline void modifyAreaIf(QRect rect, _UnaryModifierT modifier, _UnaryPredicateT predicate) {
        modifyAreaIf(rect.topLeft(), rect.size(), modifier, predicate);
    }

    [[nodiscard]] std::vector<QPoint> findAll(qint32 x, qint32 y, qint32 width, qint32 height, const _TileT& tile) const {
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
    [[nodiscard]] inline std::vector<QPoint> findAll(QPoint pos, QSize size, const _TileT& tile) const { return findAll(pos.x(), pos.y(), size.width(), size.height(), tile); }
    [[nodiscard]] inline std::vector<QPoint> findAll(QRect rect, const _TileT& tile) const { return findAll(rect.topLeft(), rect.size(), tile); }
    [[nodiscard]] inline std::vector<QPoint> findAll(const _TileT& tile) const { return findAll(0, 0, constants::_chunk_size, constants::_chunk_size, tile); }

public:
    std::array<_TileT, constants::_chunk_size * constants::_chunk_size> tiles{};

private:
    quint32 id;
};

}
