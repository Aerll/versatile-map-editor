#pragma once

#include <vt/debug/error.hpp>
#include <vt/util/token.hpp>

#include <utility>
#include <vector>

class QTextStream;

namespace vt::io {

class DDLTokenizer {
protected:
    using tokenlist_type = std::vector<std::pair<util::Token, qint32>>;
    using errorlist_type = std::vector<debug::Error>;

protected:
    void finalize();
    void validate();

public:
    void tokenize(QTextStream& text_stream);

public:
    tokenlist_type tokens;
    errorlist_type errors;
};

}
