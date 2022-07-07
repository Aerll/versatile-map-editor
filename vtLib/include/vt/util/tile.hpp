#pragma once

#include <vt/core/tile.hpp>
#include <vt/util/enums.hpp>

#include <QtGlobal>
#include <QPoint>

namespace vt::util {

void rotateTile(core::Tile& tile, enums::Rotate direction) noexcept;
void mirrorTile(core::Tile& tile, enums::Mirror axis) noexcept;



constexpr QPoint toTilesetCoords(quint8 index) noexcept {
    return { index / 16, index % 16 };
}

}
