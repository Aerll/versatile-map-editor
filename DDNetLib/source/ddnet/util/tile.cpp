#include <ddnet/util/tile.hpp>

namespace ddnet::util {

bool canRotate(const Tile& tile) noexcept {
    // TODO
    return {};
}

bool canMirror(const Tile& tile) noexcept {
    // TODO
    return {};
}

bool isGameEntity(const Tile& tile) noexcept {
    // TODO
    return false;
}

bool isFrontEntity(const Tile& tile) noexcept {
    // TODO
    return false;
}

bool isTeleEntity(const Tile& tile) noexcept {
    // TODO
    return false;
}

bool isTuneEntity(const Tile& tile) noexcept {
    // TODO
    return false;
}

bool isSwitchEntity(const Tile& tile) noexcept {
    // TODO
    return false;
}

bool isSpeedupEntity(const Tile& tile) noexcept {
    // TODO
    return false;
}

void rotateTile(Tile& tile, enums::Rotate direction) noexcept {
    switch (direction) {
        case enums::Rotate::Clockwise:
            if (static_cast<bool>(tile.rotation & enums::Rotation::R))
                tile.rotation ^= enums::Rotation::VH;

            break;

        case enums::Rotate::CounterClockwise:
            if (!static_cast<bool>(tile.rotation & enums::Rotation::R))
                tile.rotation ^= enums::Rotation::VH;

            break;
    }
    tile.rotation ^= enums::Rotation::R;
}

void mirrorTile(Tile& tile, enums::Mirror axis) noexcept {
    switch (axis) {
        case enums::Mirror::VerticalAxis:
            if (static_cast<bool>(tile.rotation & enums::Rotation::R))
                tile.rotation ^= enums::Rotation::H;
            else
                tile.rotation ^= enums::Rotation::V;

            break;

        case enums::Mirror::HorizontalAxis:
            if (static_cast<bool>(tile.rotation & enums::Rotation::R))
                tile.rotation ^= enums::Rotation::V;
            else
                tile.rotation ^= enums::Rotation::H;

            break;
    }
}

}
