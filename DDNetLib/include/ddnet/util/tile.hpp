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

void rotateTile(map::Tile& tile, Rotate direction) noexcept;
void mirrorTile(map::Tile& tile, Mirror axis) noexcept;



constexpr QPoint toTilesetCoords(quint8 index) noexcept {
    return { index / 16, index % 16 };
}

} // ddnet::util::
