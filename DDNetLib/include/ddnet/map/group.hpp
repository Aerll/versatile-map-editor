#pragma once

#include <ddnet/map/layer.hpp>

#include <QString>

#include <memory>
#include <vector>

namespace ddnet::map {

class Group {
public:
    QString name;
    std::vector<Group> groups;
    std::vector<std::shared_ptr<BaseLayer>> layers;
    bool is_visible = false;
};

} // ddnet::map::
