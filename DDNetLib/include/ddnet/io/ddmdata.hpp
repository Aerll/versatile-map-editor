#pragma once

#include <ddnet/map/map.hpp>
#include <ddnet/util/macros.hpp>

namespace ddnet::io {

class MAPData;

class DDMData {
public:
    MAPData toMAP() const;

public:
    map::Map map;
};

} // ddnet::io::
