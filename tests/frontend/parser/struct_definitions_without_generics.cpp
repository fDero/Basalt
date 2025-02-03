
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Struct_Definition_With_No_Fields_And_No_Nested_Structs) {
    std::vector<Token> tokens = {
        { struct_keyword, "test.basalt", 1, 1, 1, Token::Type::struct_keyword },
        { "S",            "test.basalt", 1, 2, 7, Token::Type::type           },
        { "{",            "test.basalt", 1, 3, 8, Token::Type::symbol         },
        { "}",            "test.basalt", 1, 4, 9, Token::Type::symbol         }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    StructDefinition struct_def = parser.parse_struct_definition();
    EXPECT_EQ(struct_def.filename, "test.basalt");
    EXPECT_EQ(struct_def.def_name, "S");
    EXPECT_TRUE(struct_def.template_generics_names.empty());
    EXPECT_TRUE(struct_def.fields.empty());
}

TEST(Frontend, Parse_Struct_Definition_With_One_Field_And_No_Nested_Structs) {
    std::vector<Token> tokens = {
        { struct_keyword, "test.basalt",  1,  1,   1,  Token::Type::struct_keyword },
        { "S",            "test.basalt",  1,  2,   7,  Token::Type::type           },
        { "{",            "test.basalt",  1,  3,   8,  Token::Type::symbol         },
        { "x",            "test.basalt",  1,  4,   9,  Token::Type::text           },
        { ":",            "test.basalt",  1,  5,  10,  Token::Type::symbol         },
        { "T",            "test.basalt",  1,  6,  11,  Token::Type::type           },
        { ";",            "test.basalt",  1,  7,  12,  Token::Type::symbol         },
        { "}",            "test.basalt",  1,  8,  13,  Token::Type::symbol         }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    StructDefinition struct_def = parser.parse_struct_definition();
    EXPECT_EQ(struct_def.filename, "test.basalt");
    EXPECT_EQ(struct_def.def_name, "S");
    EXPECT_TRUE(struct_def.template_generics_names.empty());
    ASSERT_EQ(struct_def.fields.size(), 1);
    EXPECT_EQ(struct_def.fields.back().field_name, "x");
}

TEST(Frontend, Parse_Struct_Definition_With_Two_Fields_And_No_Nested_Structs) {
    std::vector<Token> tokens = {
        { struct_keyword, "test.basalt",  1,   1,  1,   Token::Type::struct_keyword },
        { "S",      "test.basalt",  1,   2,  7,   Token::Type::type           },
        { "{",      "test.basalt",  1,   3,  8,   Token::Type::symbol         },
        { "x",      "test.basalt",  1,   4,  9,   Token::Type::text           },
        { ":",      "test.basalt",  1,   5,  10,  Token::Type::symbol         },
        { "T",      "test.basalt",  1,   6,  11,  Token::Type::type           },
        { ";",      "test.basalt",  1,   7,  12,  Token::Type::symbol         },
        { "y",      "test.basalt",  1,   8,  13,  Token::Type::text           },
        { ":",      "test.basalt",  1,   9,  14,  Token::Type::symbol         },
        { "T",      "test.basalt",  1,  10,  15,  Token::Type::type           },
        { ";",      "test.basalt",  1,  11,  16,  Token::Type::symbol         },
        { "}",      "test.basalt",  1,  12,  17,  Token::Type::symbol         }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    StructDefinition struct_def = parser.parse_struct_definition();
    EXPECT_EQ(struct_def.filename, "test.basalt");
    EXPECT_EQ(struct_def.def_name, "S");
    EXPECT_TRUE(struct_def.template_generics_names.empty());
    ASSERT_EQ(struct_def.fields.size(), 2);
    EXPECT_EQ(struct_def.fields.front().field_name, "x");
    EXPECT_EQ(struct_def.fields.back().field_name, "y");
}