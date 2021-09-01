#pragma once

#include <ddnet/debug/error.hpp>
#include <ddnet/util/token.hpp>

#include <utility>
#include <vector>

class QTextStream;

namespace ddnet::io {

class DDLTokenizer {
protected:
    using tokenlist_type = std::vector<std::pair<util::Token, qint32>>;
    using errorlist_type = std::vector<debug::Error>;

protected:
    void finalize();
    void validate();

public:
    inline DDLTokenizer() = default;
    inline DDLTokenizer(const DDLTokenizer&) = default;
    inline DDLTokenizer(DDLTokenizer&&) = default;
    inline DDLTokenizer& operator=(const DDLTokenizer&) = default;
    inline DDLTokenizer& operator=(DDLTokenizer&&) = default;

    void tokenize(QTextStream& text_stream);

public:
    tokenlist_type tokens;
    errorlist_type errors;
};

} // ddnet::io::
