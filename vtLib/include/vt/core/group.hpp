#pragma once

#include <vt/core/layer.hpp>

#include <QString>

#include <memory>
#include <vector>

namespace vt::core {

class Group {
public:
    QString name;
    std::vector<Group> groups;
    std::vector<std::shared_ptr<BaseLayer>> layers;
    bool is_visible = false;
};

}
