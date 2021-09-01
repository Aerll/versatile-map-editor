#include <ddnet/io/ddltokenizer.hpp>

#include <QTextStream>

#include <cassert>
#include <algorithm>

namespace ddnet::io {

void DDLTokenizer::finalize() {
    // concatenate string literals
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i + 3 < tokens.size()) {
            if (tokens[i + 1].first.type == util::TokenType::Literal &&
                tokens[i + 3].first.type == util::TokenType::Literal
                ) {
                tokens[i + 1].first.string += tokens[i + 3].first.string;
                tokens.erase(std::begin(tokens) + (i + 2), std::begin(tokens) + (i + 4));
                --i;
            }
        }
    }

    for (auto& token : tokens)
        if (!token.first.string.isEmpty() && token.first.string.back() == '\n') // remove the last character, only if it's a new line
            token.first.string.remove(token.first.string.size() - 1, 1);
}



void DDLTokenizer::validate() {
    for (const auto& token : tokens)
        if (static_cast<bool>(token.first.type & util::TokenType::Invalid))
            errors.emplace_back(debug::Error{ 
                .message = "Invalid token '" + token.first.string + "' at line: " + QString::number(token.second) + ".\n", 
                .code = debug::ErrorCode::Tokenizer_DDL_InvalidToken 
            });
}



void DDLTokenizer::tokenize(QTextStream& text_stream) {
    assert(tokens.empty());
    
    quint32 line = 1;
    text_stream.seek(0);
    while (!text_stream.atEnd()) {
        QChar c;
        text_stream >> c;

        if (c == ' ' || c == '\t') { // skip whitespaces
            continue;
        }
        else if (c == '\n') {
            ++line;
            continue;
        }
        else if (c == '/') { // skip comments // and /* */
            QChar next_c;
            text_stream >> next_c;

            if (next_c == '/') { //
                while (!text_stream.atEnd()) {
                    text_stream >> c;
                    if (c == '\n') {
                        ++line;
                        break;
                    }
                }
            }
            else if (next_c == '*') { /* */
                quint32 line_token = line;
                while (!text_stream.atEnd()) {
                    text_stream >> c;
                    if (c == '*') {
                        text_stream >> next_c;
                        if (next_c == '/')
                            break;
                        text_stream.seek(text_stream.pos() - 1);
                    }
                    else if (c == '\n')
                        ++line;

                    if (text_stream.atEnd())
                        tokens.push_back({ util::Token{ .string = "/*", .type = util::TokenType::Invalid | util::TokenType::Open }, line_token });
                }
            }
            else {
                tokens.push_back({ util::Token{ .string = "/", .type = util::TokenType::Invalid }, line });
                text_stream.seek(text_stream.pos() - 1);
            }
        }
        else if (c == '=') { // operators
            tokens.push_back({ util::Token{ .string = c, .type = util::TokenType::Operator }, line });
        }
        else if (c == ':') { // literals
            tokens.push_back({ util::Token{ .string = c, .type = util::TokenType::Operator }, line });
            tokens.push_back({ util::Token{ .string = {}, .type = util::TokenType::Literal }, line });
            while (!text_stream.atEnd()) {
                text_stream >> c;
                tokens.back().first.string += c;
                if (c == '\n') {
                    ++line;
                    break;
                }
            }
        }
        else if (c == '{') { // open brackets
            tokens.push_back({ util::Token{ .string = c, .type = util::TokenType::Operator | util::TokenType::Open }, line });
        }
        else if (c == '}') { // close brackets
            tokens.push_back({ util::Token{ .string = c, .type = util::TokenType::Operator | util::TokenType::Close }, line });
        }
        else if (c.isLetterOrNumber() || c == '_' || c == '-') { // identifiers
            tokens.push_back({ util::Token{ .string = c, .type = util::TokenType::Identifier }, line });
            while (!text_stream.atEnd()) {
                text_stream >> c;
                if (!c.isLetterOrNumber() && c != '_' && c != '-') {
                    text_stream.seek(text_stream.pos() - 1);
                    break;
                }
                tokens.back().first.string += c;
            }
        }
        else {
            if (!tokens.empty() && static_cast<bool>(tokens.back().first.type & util::TokenType::Invalid))
                tokens.back().first.string += c;
            else
                tokens.push_back({ util::Token{ .string = c, .type = util::TokenType::Invalid }, line });
        }
    }
    finalize();
    validate();
}

} // ddnet::io::
