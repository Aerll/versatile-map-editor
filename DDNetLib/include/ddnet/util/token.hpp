#pragma once

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

inline TokenType operator&(TokenType lhs, TokenType rhs) { return static_cast<TokenType>(static_cast<qint32>(lhs) & static_cast<qint32>(rhs)); }
inline TokenType operator&=(TokenType& lhs, TokenType rhs) { lhs = lhs & rhs; return lhs; }
inline TokenType operator|(TokenType lhs, TokenType rhs) { return static_cast<TokenType>(static_cast<qint32>(lhs) | static_cast<qint32>(rhs)); }
inline TokenType operator|=(TokenType& lhs, TokenType rhs) { lhs = lhs | rhs; return lhs; }



class Token {
public:
    inline bool operator==(const Token&) const = default;

public:
    QString string;
    TokenType type;
};

} // ddnet::util::
