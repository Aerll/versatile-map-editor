#include <vt/core/tile.hpp>

namespace vt::core {

void Tile::rotate(enums::Rotate direction) noexcept {
    switch (direction) {
        case enums::Rotate::Clockwise:
            if (static_cast<bool>(rotation & enums::Rotation::R))
                rotation ^= enums::Rotation::VH;

            break;

        case enums::Rotate::CounterClockwise:
            if (!static_cast<bool>(rotation & enums::Rotation::R))
                rotation ^= enums::Rotation::VH;

            break;
    }
    rotation ^= enums::Rotation::R;
}

void Tile::mirror(enums::Mirror axis) noexcept {
    switch (axis) {
        case enums::Mirror::VerticalAxis:
            if (static_cast<bool>(rotation & enums::Rotation::R))
                rotation ^= enums::Rotation::H;
            else
                rotation ^= enums::Rotation::V;

            break;

        case enums::Mirror::HorizontalAxis:
            if (static_cast<bool>(rotation & enums::Rotation::R))
                rotation ^= enums::Rotation::V;
            else
                rotation ^= enums::Rotation::H;

            break;
    }
}

}
