#pragma once

#include <ddnet/map/tile.hpp>

#include <QtGlobal>
#include <QPoint>

namespace ddnet {

enum class Rotate {
    Clockwise,
    CounterClockwise,
};

enum class Mirror {
    VerticalAxis,
    HorizontalAxis,
};

} // ddnet::



namespace ddnet::util {

bool canRotate(const map::Tile& tile) noexcept;
bool canMirror(const map::Tile& tile) noexcept;
bool isGameEntity(const map::Tile& tile) noexcept;
bool isFrontEntity(const map::Tile& tile) noexcept;
bool isTeleEntity(const map::Tile& tile) noexcept;
bool isTuneEntity(const map::Tile& tile) noexcept;
bool isSwitchEntity(const map::Tile& tile) noexcept;
bool isSpeedupEntity(const map::Tile& tile) noexcept;



inline constexpr QPoint toTilesetCoords(quint8 index) noexcept {
    return { index / 16, index % 16 };
}

inline void rotateTile(map::Tile& tile, Rotate direction) noexcept {
    switch (direction) {
        case Rotate::Clockwise:
            if (static_cast<bool>(tile.rotation & Rotation::R))
                tile.rotation ^= Rotation::VH;

            break;

        case Rotate::CounterClockwise:
            if (!static_cast<bool>(tile.rotation & Rotation::R))
                tile.rotation ^= Rotation::VH;

            break;
    }
    tile.rotation ^= Rotation::R;
}

inline void mirrorTile(map::Tile& tile, Mirror axis) noexcept {
    switch (axis) {
        case Mirror::VerticalAxis:
            if (static_cast<bool>(tile.rotation & Rotation::R))
                tile.rotation ^= Rotation::H;
            else
                tile.rotation ^= Rotation::V;

            break;

        case Mirror::HorizontalAxis:
            if (static_cast<bool>(tile.rotation & Rotation::R))
                tile.rotation ^= Rotation::V;
            else
                tile.rotation ^= Rotation::H;

            break;
    }
}

} // ddnet::util::
