#include <ddnet/io/ddlparser.hpp>

#include <ddnet/debug/error_message.hpp>
#include <ddnet/debug/error.hpp>
#include <ddnet/io/ddltokenizer.hpp>
#include <ddnet/util/enums.hpp>

namespace ddnet::io {

void DDLParser::parseIdentifier(const std::pair<util::Token, qint32>& token) {
    if (token.first.type != enums::TokenType::Identifier) [[unlikely]]
        errors.emplace_back(debug::Error{
            .message = debug::errorMessage<debug::ErrorCode::Parser_DDL_MissingIdentifier>(token.second),
            .code = debug::ErrorCode::Parser_DDL_MissingIdentifier
        });
}



void DDLParser::parseAssignment(const std::pair<util::Token, qint32>& token) {
    if (token.first != util::Token{ .string = "=", .type = enums::TokenType::Operator }) [[unlikely]]
        errors.emplace_back(debug::Error{
            .message = debug::errorMessage<debug::ErrorCode::Parser_DDL_MissingAssignment>(token.second),
            .code = debug::ErrorCode::Parser_DDL_MissingAssignment
        });
}



void DDLParser::parseLiteralOperator(const std::pair<util::Token, qint32>& token) {
    if (token.first != util::Token{ .string = ":", .type = enums::TokenType::Operator }) [[unlikely]]
        errors.emplace_back(debug::Error{
            .message = debug::errorMessage<debug::ErrorCode::Parser_DDL_MissingLiteralOperator>(token.second),
            .code = debug::ErrorCode::Parser_DDL_MissingLiteralOperator
        });
}



void DDLParser::errorMissingOpenBracket(const std::pair<util::Token, qint32>& token) {
    errors.emplace_back(debug::Error{
        .message = debug::errorMessage<debug::ErrorCode::Parser_DDL_MissingOpenBracket>(token.second),
        .code = debug::ErrorCode::Parser_DDL_MissingOpenBracket
    });
}



void DDLParser::errorMissingCloseBracket(const std::pair<util::Token, qint32>& token) {
    errors.emplace_back(debug::Error{
        .message = debug::errorMessage<debug::ErrorCode::Parser_DDL_MissingCloseBracket>(token.second),
        .code = debug::ErrorCode::Parser_DDL_MissingCloseBracket
    });
}



void DDLParser::errorUnexpectedToken(const std::pair<util::Token, qint32>& token) {
    errors.emplace_back(debug::Error{
        .message = debug::errorMessage<debug::ErrorCode::Parser_DDL_UnexpectedToken>(token.first.string, token.second),
        .code = debug::ErrorCode::Parser_DDL_UnexpectedToken
    });
}



void DDLParser::parse(const tokenlist_type& tokens) {
    // - true : parsing attributes
    // - false: parsing properties
    bool parse_attributes = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i == 0) { // first token
            parseIdentifier(tokens[i]);
            if (tokens[i].first.type != enums::TokenType::Identifier)
                errorUnexpectedToken(tokens[i]);
        }

        if (tokens[i].first == util::Token{ .string = "{", .type = enums::TokenType::Operator | enums::TokenType::Open })
            parse_attributes = true;
        else if (tokens[i].first == util::Token{ .string = "}", .type = enums::TokenType::Operator | enums::TokenType::Close })
            parse_attributes = false;
        
        if (i == tokens.size() - 1) { // last token
            if (tokens[i].first != util::Token{ .string = "}", .type = enums::TokenType::Operator | enums::TokenType::Close }) {
                errorMissingCloseBracket(tokens[i]);
                continue;
            }
        }
        else { // rest
            if (tokens[i + 1].first == util::Token{ .string = "=", .type = enums::TokenType::Operator }
                ) { // 'id ='
                parseIdentifier(tokens[i]);
                continue;
            }
            else if (
                tokens[i + 1].first == util::Token{ .string = ":", .type = enums::TokenType::Operator }
                ) { // '= :'
                parseAssignment(tokens[i]);
                continue;
            }
            else if (
                tokens[i].first == util::Token{ .string = "=", .type = enums::TokenType::Operator }
                ) { // '= :'
                parseLiteralOperator(tokens[i + 1]);
                continue;
            }
            else if (
                tokens[i + 1].first == util::Token{ .string = "{", .type = enums::TokenType::Operator | enums::TokenType::Open }
                ) { // ': {' & 'literal {'
                if (tokens[i].first.type != enums::TokenType::Literal) {
                    errorUnexpectedToken(tokens[i + 1]);
                    continue;
                }
                else if (parse_attributes) {
                    errorMissingCloseBracket(tokens[i]);
                    continue;
                }
            }
            else if (
                tokens[i].first.type == enums::TokenType::Literal &&
                tokens[i + 1].first != util::Token{ .string = "{", .type = enums::TokenType::Operator | enums::TokenType::Open } &&
                !parse_attributes
                ) { // ': [inv]' & 'literal [inv]'
                errorMissingOpenBracket(tokens[i]);
                errorUnexpectedToken(tokens[i + 1]);
                continue;
            }
            else if (
                tokens[i + 1].first == util::Token{ .string = "}", .type = enums::TokenType::Operator | enums::TokenType::Close }
                ) { // ': }' & 'literal }' & '{ }'
                if (tokens[i].first.type != enums::TokenType::Literal &&
                    tokens[i].first != util::Token{ .string = "{", .type = enums::TokenType::Operator | enums::TokenType::Open }
                    ) {
                    errorUnexpectedToken(tokens[i + 1]);
                    continue;
                }
            }
        }
    }
}

}
