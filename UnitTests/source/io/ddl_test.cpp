#include <catch2.pch>

#include <ddnet/io/ddlfilestream.hpp>
#include <ddnet/io/ddltokenizer.hpp>
#include <ddnet/io/ddlparser.hpp>
#include <ddnet/io/ddldata.hpp>
#include <ddnet/util/constants.hpp>

#include <QFileInfo>
#include <QTextStream>

#include <algorithm>
#include <string_view>

using namespace ddnet;

namespace test_util {

bool hasError(const io::DDLParser& parser, debug::ErrorCode code) {
    auto it = std::ranges::find_if(parser.errors, [code](const auto& error) {
        return error.code == code;
    });
    return it != parser.errors.end();
}

} // test_util::

TEST_CASE("io::DDLData") {
    SECTION("Number of properties") {
        io::DDLData data;
        data.properties[{ .name = "Prop1" }];
        data.properties[{ .name = "Prop2" }];
        data.properties[{ .name = "Prop3" }];
        data.properties[{ .name = "Prop4" }];
        data.properties[{ .name = "Prop5" }];
        CHECK(data.propertiesCount() == 5);
    }

    SECTION("Number of attributes in given property") {
        io::DDLData data;
        std::map<QString, QString> attributes1;
        std::map<QString, QString> attributes2;

        data.properties[{ .name = "Prop1", .value = "Value1" }] = {
            { "Attr1", "Value" },
            { "Attr2", "Value" },
            { "Attr3", "Value" },
        };
        data.properties[{ .name = "Prop2", .value = "Value2" }] = {
            { "Attr1", "Value" },
            { "Attr2", "Value" },
            { "Attr3", "Value" },
            { "Attr4", "Value" },
            { "Attr5", "Value" },
        };

        CHECK(data.attributesCount({ .name = "Prop1", .value = "Value1" }) == 3);
        CHECK(data.attributesCount(std::pair<QString, QString>{ "Prop2", "Value2" }) == 5);
        CHECK(data.attributesCount({ .name = "invalid", .value = "invalid" }) == constants::_notfound);
    }

    SECTION("Get the value of a given property") {
        io::DDLData data;
        data.properties[{ .name = "Prop1", .value = "Value1" }];
        data.properties[{ .name = "Prop2", .value = "Value2" }];
        data.properties[{ .name = "Prop3", .value = "Value3" }];

        CHECK(data.property({ "Prop1", "Value1" })->first.value == "Value1");
        CHECK(data.property({ "Prop2", "Value2" })->first.value == "Value2");
        CHECK(data.property({ "Prop3", "Value3" })->first.value == "Value3");
        CHECK(data.property({ "invalid", "invalid" }) == data.cend());
    }
}

TEST_CASE("io::DDLTokenizer") {
    SECTION("Line comments") {
        QString ddl_sample =
            "// line comment test" "\n"
            "// some // comment"   "\n"
            "// //"                "\n"
            ""                     "\n"
            "//"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.empty());
    }

    SECTION("Invalid line comment") {
        QString ddl_sample =
            "// good" "\n"
            "/ "
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.size() == 1);

        CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "/", util::TokenType::Invalid });

        CHECK(ddl_tokenizer.tokens[0].second == 2);
    }

    SECTION("Block comments") {
        QString ddl_sample =
            "/* this"    "\n"
            "/*is"       "\n"
            "a*"         "\n"
            "*block"     "\n"
            "comment**/" "\n"
            ""           "\n"
            "/**/"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.empty());
    }

    SECTION("Invalid block comment") {
        QString ddl_sample =
            "/* good" "\n"
            "*/"      "\n"
            "/* not"  "\n"
            "closed"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.size() == 1);

        CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "/*", util::TokenType::Invalid | util::TokenType::Open });

        CHECK(ddl_tokenizer.tokens[0].second == 3);
    }

    SECTION("Operators") {
        QString ddl_sample =
            "= :"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.size() == 3);

        CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[1].first == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[2].first == util::Token{ {}, util::TokenType::Literal });

        CHECK(ddl_tokenizer.tokens[0].second == 1);
        CHECK(ddl_tokenizer.tokens[1].second == 1);
        CHECK(ddl_tokenizer.tokens[2].second == 1);
    }

    SECTION("Brackets") {
        QString ddl_sample =
            "{" "\n"
            " " "\n"
            "}"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.size() == 2);

        CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "{", util::TokenType::Operator | util::TokenType::Open });
        CHECK(ddl_tokenizer.tokens[1].first == util::Token{ "}", util::TokenType::Operator | util::TokenType::Close });

        CHECK(ddl_tokenizer.tokens[0].second == 1);
        CHECK(ddl_tokenizer.tokens[1].second == 3);
    }

    SECTION("Identifiers") {
        QString ddl_sample =
            "Identifier_1 Identifier_2" "\n"
            "Ident=ifier"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.size() == 5);

        CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "Identifier_1", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[1].first == util::Token{ "Identifier_2", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[2].first == util::Token{ "Ident", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[3].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[4].first == util::Token{ "ifier", util::TokenType::Identifier });

        CHECK(ddl_tokenizer.tokens[0].second == 1);
        CHECK(ddl_tokenizer.tokens[1].second == 1);
        CHECK(ddl_tokenizer.tokens[2].second == 2);
        CHECK(ddl_tokenizer.tokens[3].second == 2);
        CHECK(ddl_tokenizer.tokens[4].second == 2);
    }

    SECTION("Literals") {
        { // test 1
            QString ddl_sample =
                ":" "\n" // \n is discarded
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            CHECK(text_stream.atEnd());
            REQUIRE(ddl_tokenizer.tokens.size() == 2);

            CHECK(ddl_tokenizer.tokens[0].first == util::Token{ ":", util::TokenType::Operator });
            CHECK(ddl_tokenizer.tokens[1].first == util::Token{ "", util::TokenType::Literal });

            CHECK(ddl_tokenizer.tokens[0].second == 1);
            CHECK(ddl_tokenizer.tokens[1].second == 1);
        }

        { // test 2
            QString ddl_sample =
                ":literal1"         "\n"
                ":literal2"         "\n"
                ":!@#$%^&*()///**/" "\n" // \n is discarded
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            CHECK(text_stream.atEnd());
            REQUIRE(ddl_tokenizer.tokens.size() == 2);

            CHECK(ddl_tokenizer.tokens[0].first == util::Token{ ":", util::TokenType::Operator });
            CHECK(ddl_tokenizer.tokens[1].first == util::Token{ "literal1\nliteral2\n!@#$%^&*()///**/", util::TokenType::Literal });

            CHECK(ddl_tokenizer.tokens[0].second == 1);
            CHECK(ddl_tokenizer.tokens[1].second == 1);
        }

        { // test 3
            QString ddl_sample =
                "::"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            CHECK(text_stream.atEnd());
            REQUIRE(ddl_tokenizer.tokens.size() == 2);

            CHECK(ddl_tokenizer.tokens[0].first == util::Token{ ":", util::TokenType::Operator });
            CHECK(ddl_tokenizer.tokens[1].first == util::Token{ ":", util::TokenType::Literal });

            CHECK(ddl_tokenizer.tokens[0].second == 1);
            CHECK(ddl_tokenizer.tokens[1].second == 1);
        }
    }

    SECTION("Invalid") {
        { // test 1
            QString ddl_sample =
                "Invalid1@"   "\n"
                "Invalid2#"   "\n"
                "Invalid3!@#"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            CHECK(text_stream.atEnd());
            REQUIRE(ddl_tokenizer.tokens.size() == 6);

            CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "Invalid1", util::TokenType::Identifier });
            CHECK(ddl_tokenizer.tokens[1].first == util::Token{ "@", util::TokenType::Invalid });
            CHECK(ddl_tokenizer.tokens[2].first == util::Token{ "Invalid2", util::TokenType::Identifier });
            CHECK(ddl_tokenizer.tokens[3].first == util::Token{ "#", util::TokenType::Invalid });
            CHECK(ddl_tokenizer.tokens[4].first == util::Token{ "Invalid3", util::TokenType::Identifier });
            CHECK(ddl_tokenizer.tokens[5].first == util::Token{ "!@#", util::TokenType::Invalid });

            CHECK(ddl_tokenizer.tokens[0].second == 1);
            CHECK(ddl_tokenizer.tokens[1].second == 1);
            CHECK(ddl_tokenizer.tokens[2].second == 2);
            CHECK(ddl_tokenizer.tokens[3].second == 2);
            CHECK(ddl_tokenizer.tokens[4].second == 3);
            CHECK(ddl_tokenizer.tokens[5].second == 3);
        }
        
        { // test 2
            QString ddl_sample =
                "@"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            CHECK(text_stream.atEnd());
            REQUIRE(ddl_tokenizer.tokens.size() == 1);

            CHECK(ddl_tokenizer.tokens[0].first == util::Token{ "@", util::TokenType::Invalid });

            CHECK(ddl_tokenizer.tokens[0].second == 1);
        }
    }

    SECTION("DDL sample") {
        QString ddl_sample =
            "Test1 = :test\n"
            "{\n"
            "    Something = :hello\n"
            "    SomethingElse\n"
            "    =\n"
            "    :world\n"
            "}\n"
            "\n"
            "UsingTest1 = :val\n"
            "{\n"
            "    Test1 = :test\n"
            "    Something = :hello\n"
            "                :world\n"
            "}\n"
            "\n"
            "Empty = :\n"
            "{\n"
            "}\n"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        CHECK(text_stream.atEnd());
        REQUIRE(ddl_tokenizer.tokens.size() == 34);

        CHECK(ddl_tokenizer.tokens[0].first  == util::Token{ "Test1", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[1].first  == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[2].first  == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[3].first  == util::Token{ "test", util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[4].first  == util::Token{ "{", util::TokenType::Operator | util::TokenType::Open });
        CHECK(ddl_tokenizer.tokens[5].first  == util::Token{ "Something", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[6].first  == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[7].first  == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[8].first  == util::Token{ "hello", util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[9].first  == util::Token{ "SomethingElse", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[10].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[11].first == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[12].first == util::Token{ "world", util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[13].first == util::Token{ "}", util::TokenType::Operator | util::TokenType::Close });
        CHECK(ddl_tokenizer.tokens[14].first == util::Token{ "UsingTest1", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[15].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[16].first == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[17].first == util::Token{ "val", util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[18].first == util::Token{ "{", util::TokenType::Operator | util::TokenType::Open });
        CHECK(ddl_tokenizer.tokens[19].first == util::Token{ "Test1", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[20].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[21].first == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[22].first == util::Token{ "test", util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[23].first == util::Token{ "Something", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[24].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[25].first == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[26].first == util::Token{ "hello\nworld", util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[27].first == util::Token{ "}", util::TokenType::Operator | util::TokenType::Close });
        CHECK(ddl_tokenizer.tokens[28].first == util::Token{ "Empty", util::TokenType::Identifier });
        CHECK(ddl_tokenizer.tokens[29].first == util::Token{ "=", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[30].first == util::Token{ ":", util::TokenType::Operator });
        CHECK(ddl_tokenizer.tokens[31].first == util::Token{ {}, util::TokenType::Literal });
        CHECK(ddl_tokenizer.tokens[32].first == util::Token{ "{", util::TokenType::Operator | util::TokenType::Open });
        CHECK(ddl_tokenizer.tokens[33].first == util::Token{ "}", util::TokenType::Operator | util::TokenType::Close });

        CHECK(ddl_tokenizer.tokens[0].second  == 1);
        CHECK(ddl_tokenizer.tokens[1].second  == 1);
        CHECK(ddl_tokenizer.tokens[2].second  == 1);
        CHECK(ddl_tokenizer.tokens[3].second  == 1);
        CHECK(ddl_tokenizer.tokens[4].second  == 2);
        CHECK(ddl_tokenizer.tokens[5].second  == 3);
        CHECK(ddl_tokenizer.tokens[6].second  == 3);
        CHECK(ddl_tokenizer.tokens[7].second  == 3);
        CHECK(ddl_tokenizer.tokens[8].second  == 3);
        CHECK(ddl_tokenizer.tokens[9].second  == 4);
        CHECK(ddl_tokenizer.tokens[10].second == 5);
        CHECK(ddl_tokenizer.tokens[11].second == 6);
        CHECK(ddl_tokenizer.tokens[12].second == 6);
        CHECK(ddl_tokenizer.tokens[13].second == 7);
        CHECK(ddl_tokenizer.tokens[14].second == 9);
        CHECK(ddl_tokenizer.tokens[15].second == 9);
        CHECK(ddl_tokenizer.tokens[16].second == 9);
        CHECK(ddl_tokenizer.tokens[17].second == 9);
        CHECK(ddl_tokenizer.tokens[18].second == 10);
        CHECK(ddl_tokenizer.tokens[19].second == 11);
        CHECK(ddl_tokenizer.tokens[20].second == 11);
        CHECK(ddl_tokenizer.tokens[21].second == 11);
        CHECK(ddl_tokenizer.tokens[22].second == 11);
        CHECK(ddl_tokenizer.tokens[23].second == 12);
        CHECK(ddl_tokenizer.tokens[24].second == 12);
        CHECK(ddl_tokenizer.tokens[25].second == 12);
        CHECK(ddl_tokenizer.tokens[26].second == 12);
        CHECK(ddl_tokenizer.tokens[27].second == 14);
        CHECK(ddl_tokenizer.tokens[28].second == 16);
        CHECK(ddl_tokenizer.tokens[29].second == 16);
        CHECK(ddl_tokenizer.tokens[30].second == 16);
        CHECK(ddl_tokenizer.tokens[31].second == 16);
        CHECK(ddl_tokenizer.tokens[32].second == 17);
        CHECK(ddl_tokenizer.tokens[33].second == 18);
    }

    SECTION("Token validation") {
        QString ddl_sample =
            "Invalid1@"   "\n"
            "Invalid2#"   "\n"
            "Invalid3!@#"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);
        
        CHECK(ddl_tokenizer.errors.size() == 3);
        for (const auto& error : ddl_tokenizer.errors)
            CHECK(error.code == debug::ErrorCode::Tokenizer_DDL_InvalidToken);
    }
}

TEST_CASE("io::DDLParser") {
    SECTION("Missing identifier") {
        { // test 1
            QString ddl_sample =
                " = :value" "\n"
                "{"         "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingIdentifier));
        }

        { // test 2
            QString ddl_sample =
                "id = :value"   "\n"
                "{"             "\n"
                "     = :value" "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingIdentifier));
        }
    }

    SECTION("Missing assignment") {
        { // test 1
            QString ddl_sample =
                "id :value" "\n"
                "{"         "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingAssignment));
        }

        { // test 2
            QString ddl_sample =
                "id :value" "\n"
                "   :other" "\n"
                "{"         "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingAssignment));
        }
    }

    SECTION("Missing string literal operator") {
        { // test 1
            QString ddl_sample =
                "id = value" "\n"
                "{"          "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingLiteralOperator));
        }

        { // test 2
            QString ddl_sample =
                "id = " "\n"
                "{"     "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingLiteralOperator));
        }
    }

    SECTION("Missing open bracket") {
        { // test 1
            QString ddl_sample =
                "id = :" "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingOpenBracket));
        }

        { // test 2
            QString ddl_sample =
                "id1 = :" "\n"
                "id2 = :" "\n"
                "{"       "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingOpenBracket));
        }
    }

    SECTION("Missing close bracket") {
        { // test 1
            QString ddl_sample =
                "id = :" "\n"
                "{"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingCloseBracket));
        }
        
        { // test 2
            QString ddl_sample =
                "id1 = :" "\n"
                "{"       "\n"
                "id2 = :" "\n"
                "{"       "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingCloseBracket));
        }
        
        { // test 2
            QString ddl_sample =
                "id1 = :"   "\n"
                "{"         "\n"
                "attr1 = :" "\n"
                "attr2 = :" "\n"
                "id2 = :"   "\n"
                "{"         "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_MissingCloseBracket));
        }
    }

    SECTION("Unexpected token") {
        { // test 1
            QString ddl_sample =
                "{"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_UnexpectedToken));
        }

        { // test 2
            QString ddl_sample =
                "id = :"  "\n"
                "inv = :" "\n"
                "{"       "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_UnexpectedToken));
        }

        { // test 3
            QString ddl_sample =
                "id = :" "\n"
                "{}"     "\n"
                "}"
                ;
            QTextStream text_stream{ &ddl_sample };

            io::DDLTokenizer ddl_tokenizer;
            ddl_tokenizer.tokenize(text_stream);

            io::DDLParser ddl_parser;
            ddl_parser.parse(ddl_tokenizer.tokens);

            CHECK(test_util::hasError(ddl_parser, debug::ErrorCode::Parser_DDL_UnexpectedToken));
        }
    }

    SECTION("DDL Sample") {
        QString ddl_sample =
            "Test1 = :test\n"
            "{\n"
            "    Something = :hello\n"
            "    SomethingElse\n"
            "    =\n"
            "    :world\n"
            "}\n"
            "\n"
            "UsingTest1 = :val\n"
            "{\n"
            "    Test1 = :test\n"
            "    Something = :hello\n"
            "                :world\n"
            "}\n"
            "\n"
            "Empty = :\n"
            "{\n"
            "}\n"
            ;
        QTextStream text_stream{ &ddl_sample };

        io::DDLTokenizer ddl_tokenizer;
        ddl_tokenizer.tokenize(text_stream);

        io::DDLParser ddl_parser;
        ddl_parser.parse(ddl_tokenizer.tokens);

        CHECK(ddl_parser.errors.empty());
    }
}

TEST_CASE("io::DDLFileStream") {
    SECTION("Read/Write properties and attributes from/to a file") {
        { // test 1
            // load
            io::DDLFileStream file_stream;
            auto error_code = file_stream.loadFile({ "data/ddl/input_file1.ddl" });

            REQUIRE(error_code == debug::ErrorCode::NoError);
            REQUIRE(file_stream.data.propertiesCount() == 3);
            REQUIRE(file_stream.data.attributesCount(std::pair<QString, QString>{ "Test1", "test" }) == 2);
            REQUIRE(file_stream.data.attributesCount(std::pair<QString, QString>{ "UsingTest1", "val" }) == 2);
            REQUIRE(file_stream.data.attributesCount(std::pair<QString, QString>{ "Empty", "" }) == 0);

            auto property = file_stream.data.property(std::pair<QString, QString> {
                "Test1",
                file_stream.data.properties[io::DDLProperty{ .name = "UsingTest1", .value = "val" }]["Test1"]
            });
            REQUIRE(property->second.size() == 2);
            CHECK(property->first.name == "Test1");
            CHECK(property->first.value == "test");
            CHECK(property->second.contains("Something"));
            CHECK(property->second.contains("SomethingElse"));

            // save
            error_code = file_stream.saveFile({ "data/ddl/output_file1.ddl" });

            REQUIRE(error_code == debug::ErrorCode::NoError);

            io::DDLFileStream file_stream2;
            error_code = file_stream2.loadFile({ "data/ddl/output_file1.ddl" });

            REQUIRE(error_code == debug::ErrorCode::NoError);
            CHECK(file_stream.data.properties == file_stream2.data.properties);
        }

        { // test 2
            // load
            io::DDLFileStream file_stream;
            auto error_code = file_stream.loadFile({ "data/ddl/input_file2.ddl" });

            REQUIRE(error_code == debug::ErrorCode::NoError);
            REQUIRE(file_stream.data.propertiesCount() == 3);
            REQUIRE(file_stream.data.attributesCount(std::pair<QString, QString>{ "Test", "value1" }) == 2);
            REQUIRE(file_stream.data.attributesCount(std::pair<QString, QString>{ "Test", "value2" }) == 2);
            REQUIRE(file_stream.data.attributesCount(std::pair<QString, QString>{ "Test", "value3" }) == 2);

            // save
            error_code = file_stream.saveFile({ "data/ddl/output_file2.ddl" });

            REQUIRE(error_code == debug::ErrorCode::NoError);

            io::DDLFileStream file_stream2;
            error_code = file_stream2.loadFile({ "data/ddl/output_file2.ddl" });

            REQUIRE(error_code == debug::ErrorCode::NoError);
            CHECK(file_stream.data.properties == file_stream2.data.properties);
        }
    }
}
