#pragma once

#include <vt/map/tile.hpp>

#include <QString>

namespace vt::util::ddnet {

// TODO: bool canRotate(const Tile& tile, _T layer_type) noexcept;
// TODO: bool canMirror(const Tile& tile, _T layer_type) noexcept;
bool isGameEntity(const Tile& tile) noexcept;
bool isFrontEntity(const Tile& tile) noexcept;
bool isTeleEntity(const Tile& tile) noexcept;
bool isTuneEntity(const Tile& tile) noexcept;
bool isSwitchEntity(const Tile& tile) noexcept;
bool isSpeedupEntity(const Tile& tile) noexcept;
bool isServerSetting(const QString& string) noexcept;

}
