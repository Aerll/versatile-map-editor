#pragma once

#include <vt/util/enums.hpp>

#include <QString>

namespace vt::util {

struct Token {
    inline bool operator==(const Token&) const = default;

    QString string;
    enums::TokenType type;
};

}
