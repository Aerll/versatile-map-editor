#pragma once

#include <ddnet/util/macros.hpp>

#include <QString>

namespace ddnet::util {

enum class TokenType {
    Invalid    = 1 << 0,
    Identifier = 1 << 1,
    Literal    = 1 << 2,
    Keyword    = 1 << 3,
    Operator   = 1 << 4,
    Open       = 1 << 5,
    Close      = 1 << 6,
    Terminal   = 1 << 7,
};
ENUM_DEFINE_AND_(TokenType);
ENUM_DEFINE_OR_(TokenType);



class Token {
public:
    inline bool operator==(const Token&) const = default;

public:
    QString string;
    TokenType type;
};

} // ddnet::util::
