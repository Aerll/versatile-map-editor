#pragma once

#include <ddnet/util/macros.hpp>
#include <ddnet/util/utility.hpp>

#include <QString>

namespace ddnet::util {

enum class TokenType {
    Invalid    = util::flag(0),
    Identifier = util::flag(1),
    Literal    = util::flag(2),
    Keyword    = util::flag(3),
    Operator   = util::flag(4),
    Open       = util::flag(5),
    Close      = util::flag(6),
    Terminal   = util::flag(7),
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
