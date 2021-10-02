#include <ddnet/io/ddlfilestream.hpp>

#include <ddnet/io/ddltokenizer.hpp>
#include <ddnet/io/ddlparser.hpp>
#include <ddnet/util/enums.hpp>

#include <QTextStream>
#include <QFile>
#include <QFileInfo>

namespace ddnet::io {

debug::ErrorCode DDLFileStream::loadFile(const QFileInfo& file_info) {
    QFile file{ file_info.absoluteFilePath() };

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()) [[unlikely]]
        return debug::ErrorCode::File_FailedToRead;

    QTextStream ddl_text_stream{ &file };
    if (ddl_text_stream.device()->size() == 0) [[unlikely]]
        return debug::ErrorCode::File_NoData;

    DDLTokenizer tokenizer;
    tokenizer.tokenize(ddl_text_stream);
    if (!tokenizer.errors.empty()) [[unlikely]] {
        errors = tokenizer.errors;
        return debug::ErrorCode::File_DDL_FailedToTokenize;
    }

    DDLParser parser;
    parser.parse(tokenizer.tokens);
    if (!parser.errors.empty()) [[unlikely]] {
        errors = parser.errors;
        return debug::ErrorCode::File_DDL_FailedToParse;
    }

    // read data
    bool read_attributes = false;
    DDLProperty property;
    for (size_t i = 0; i < tokenizer.tokens.size(); ++i) {
        if (tokenizer.tokens[i].first == util::Token{ .string = "{", .type = enums::TokenType::Operator | enums::TokenType::Open })
            read_attributes = true;
        else if (tokenizer.tokens[i].first == util::Token{ .string = "}", .type = enums::TokenType::Operator | enums::TokenType::Close })
            read_attributes = false;

        if (tokenizer.tokens[i].first.type == enums::TokenType::Identifier) {
            if (!read_attributes) {
                property.name  = tokenizer.tokens[i].first.string;
                property.value = tokenizer.tokens[i + 3].first.string;
                data.properties.insert({ property, {} });
            }
            else {
                data.properties[property].insert(std::pair<QString, QString>{ 
                    tokenizer.tokens[i].first.string,
                    tokenizer.tokens[i + 3].first.string 
                });
            }
            i += 3;
        }
    }
    return debug::ErrorCode::NoError;
}



debug::ErrorCode DDLFileStream::saveFile(const QFileInfo& file_info) {
    QFile file{ file_info.absoluteFilePath() };

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file.isOpen()) [[unlikely]]
        return debug::ErrorCode::File_FailedToWrite;

    QTextStream ddl_text_stream{ &file };

    auto writeString = [&](const QString& string, qint32 padding) -> void {
        for (qint32 i = 0; i < string.size(); ++i) {
            ddl_text_stream << string[i];
            if (string[i] == '\n')
                ddl_text_stream << QString{ padding, ' ' } << ':';
        }
    };

    // write data
    for (const auto& [property, attributes] : data.properties) {
        ddl_text_stream << property.name << " = :";
        writeString(property.value, property.name.size() + 3);
        ddl_text_stream << "\n{";
        for (const auto& [attr_name, attr_value] : attributes) {
            ddl_text_stream << '\n';
            ddl_text_stream << "    " << attr_name << " = :";
            writeString(attr_value, 4 + attr_name.size() + 3);
        }
        ddl_text_stream << "\n}";
        ddl_text_stream << "\n\n";
    }

    return debug::ErrorCode::NoError;
}

}
