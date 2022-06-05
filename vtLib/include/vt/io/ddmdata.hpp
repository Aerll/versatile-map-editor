#pragma once

#include <vt/map/map.hpp>

namespace vt::io {

class MAPData;

class DDMData {
public:
    MAPData toMAP() const;

public:
    Map map;
};

}
