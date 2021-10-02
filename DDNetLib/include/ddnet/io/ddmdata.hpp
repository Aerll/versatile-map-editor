#pragma once

#include <ddnet/map/map.hpp>

namespace ddnet::io {

class MAPData;

class DDMData {
public:
    MAPData toMAP() const;

public:
    Map map;
};

}
