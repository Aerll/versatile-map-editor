#include <ddnet/util/tile.hpp>

namespace ddnet::util {

bool canRotate(const map::Tile& tile) noexcept {
    // TODO
    return {};
}

bool canMirror(const map::Tile& tile) noexcept {
    // TODO
    return {};
}

bool isGameEntity(const map::Tile& tile) noexcept {
    // TODO
    return false;
}

bool isFrontEntity(const map::Tile& tile) noexcept {
    // TODO
    return false;
}

bool isTeleEntity(const map::Tile& tile) noexcept {
    // TODO
    return false;
}

bool isTuneEntity(const map::Tile& tile) noexcept {
    // TODO
    return false;
}

bool isSwitchEntity(const map::Tile& tile) noexcept {
    // TODO
    return false;
}

bool isSpeedupEntity(const map::Tile& tile) noexcept {
    // TODO
    return false;
}

void rotateTile(map::Tile& tile, Rotate direction) noexcept {
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

void mirrorTile(map::Tile& tile, Mirror axis) noexcept {
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
