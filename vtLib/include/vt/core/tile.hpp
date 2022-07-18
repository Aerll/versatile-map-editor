#pragma once

#include <vt/util/enums.hpp>
#include <vt/util/common.hpp>

#include <QPoint>

namespace vt::core {

class Tile {
public:
    [[nodiscard]] inline QPoint getTilesetCoords() noexcept { return { index / 16, index % 16 }; }
    inline void select() noexcept { is_selected = true; }
    inline void unselect() noexcept { is_selected = false; }

    void rotate(enums::Rotate direction) noexcept;
    void mirror(enums::Mirror axis) noexcept;

public:
    quint8 index = 0;
    enums::Rotation rotation = enums::Rotation::N;
    bool is_selected = false;
};

}
