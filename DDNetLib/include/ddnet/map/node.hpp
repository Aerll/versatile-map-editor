#pragma once

#include <ddnet/map/tile.hpp>

#include <memory>
#include <array>

namespace ddnet::map {

class Node {
public:
    std::array<std::shared_ptr<Tile>, 256 * 256> tiles;
};

} // ddnet::map::
