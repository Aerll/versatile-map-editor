#pragma once

#include <vt/util/enums.hpp>
#include <vt/util/common.hpp>

#include <QtGlobal>

namespace vt::core {

class Tile {
public:
    quint8 index = 0;
    enums::Rotation rotation = enums::Rotation::N;
    bool is_selected = false;
};

}
