#pragma once

#include <vt/map/map.hpp>

namespace vt::io::ddnet {

class MAPData;

}



namespace vt::io {

class DDMData {
public:
    io::ddnet::MAPData toMAP() const;

public:
    Map map;
};

}
