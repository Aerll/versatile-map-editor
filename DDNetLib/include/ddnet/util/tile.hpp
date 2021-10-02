#pragma once

#include <ddnet/map/tile.hpp>
#include <ddnet/util/enums.hpp>

#include <QtGlobal>
#include <QPoint>

namespace ddnet::util {

bool canRotate(const Tile& tile) noexcept;
bool canMirror(const Tile& tile) noexcept;
bool isGameEntity(const Tile& tile) noexcept;
bool isFrontEntity(const Tile& tile) noexcept;
bool isTeleEntity(const Tile& tile) noexcept;
bool isTuneEntity(const Tile& tile) noexcept;
bool isSwitchEntity(const Tile& tile) noexcept;
bool isSpeedupEntity(const Tile& tile) noexcept;

void rotateTile(Tile& tile, enums::Rotate direction) noexcept;
void mirrorTile(Tile& tile, enums::Mirror axis) noexcept;



constexpr QPoint toTilesetCoords(quint8 index) noexcept {
    return { index / 16, index % 16 };
}

}
