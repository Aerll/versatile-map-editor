#pragma once

#include <ddnet/util/enums.hpp>

#include <QString>

namespace ddnet::util {

struct Token {
    inline bool operator==(const Token&) const = default;

    QString string;
    enums::TokenType type;
};

}
