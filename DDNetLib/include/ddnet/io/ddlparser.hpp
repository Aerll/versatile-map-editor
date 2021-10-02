#pragma once

#include <ddnet/debug/error.hpp>
#include <ddnet/util/token.hpp>

#include <utility>
#include <vector>

namespace ddnet::io {

class DDLTokenizer;

class DDLParser {
protected:
    using tokenlist_type = std::vector<std::pair<util::Token, qint32>>;
    using errorlist_type = std::vector<debug::Error>;

protected:
    void parseIdentifier(const std::pair<util::Token, qint32>& token);
    void parseAssignment(const std::pair<util::Token, qint32>& token);
    void parseLiteralOperator(const std::pair<util::Token, qint32>& token);

    void errorMissingOpenBracket(const std::pair<util::Token, qint32>& token);
    void errorMissingCloseBracket(const std::pair<util::Token, qint32>& token);

    void errorUnexpectedToken(const std::pair<util::Token, qint32>& token);

public:
    void parse(const tokenlist_type& tokens);

public:
    errorlist_type errors;
};

}
