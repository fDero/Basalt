
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Struct_Generic_Definition_With_No_Fields_And_No_Nested_Structs) {
    std::vector<Token> struct_def_tokens = {
        { "struct", "test.basalt",  1,  1,  1,   Token::Type::struct_keyword },
        { "S",      "test.basalt",  1,  2,  7,   Token::Type::type           },
        { "<",      "test.basalt",  1,  3,  8,   Token::Type::symbol         },
        { "T",      "test.basalt",  1,  2,  9,   Token::Type::type           },
        { ",",      "test.basalt",  1,  2,  10,  Token::Type::symbol         },
        { "U",      "test.basalt",  1,  2,  11,  Token::Type::type           },
        { ">",      "test.basalt",  1,  4,  12,  Token::Type::symbol         },
        { "{",      "test.basalt",  1,  3,  13,  Token::Type::symbol         },
        { "}",      "test.basalt",  1,  4,  14,  Token::Type::symbol         }
    };
    Parser parser = Parser(struct_def_tokens);
    StructDefinition struct_def = parser.parse_struct_definition();
    EXPECT_EQ(struct_def.filename, "test.basalt");
    EXPECT_EQ(struct_def.struct_name, "S");
    EXPECT_TRUE(struct_def.fields.empty());
    ASSERT_EQ(struct_def.template_generics_names.size(), 2);
    EXPECT_EQ(struct_def.template_generics_names.front(), "T");
    EXPECT_EQ(struct_def.template_generics_names.back(), "U");
}

TEST(Parsing, Struct_Generic_Definition_With_One_Field_And_No_Nested_Structs) {
    std::vector<Token> struct_def_tokens = {
        { "struct", "test.basalt",  1,  1,   1,  Token::Type::struct_keyword },
        { "S",      "test.basalt",  1,  2,   7,  Token::Type::type           },
        { "<",      "test.basalt",  1,  3,   8,  Token::Type::symbol         },
        { "T",      "test.basalt",  1,  2,   9,  Token::Type::type           },
        { ">",      "test.basalt",  1,  4,  10,  Token::Type::symbol         },
        { "{",      "test.basalt",  1,  5,  11,  Token::Type::symbol         },
        { "x",      "test.basalt",  1,  6,  12,  Token::Type::text           },
        { ":",      "test.basalt",  1,  7,  13,  Token::Type::symbol         },
        { "T",      "test.basalt",  1,  8,  14,  Token::Type::type           },
        { ";",      "test.basalt",  1,  9,  15,  Token::Type::symbol         },
        { "}",      "test.basalt",  1,  10, 16,  Token::Type::symbol         }
    };
    Parser parser = Parser(struct_def_tokens);
    StructDefinition struct_def = parser.parse_struct_definition();
    EXPECT_EQ(struct_def.filename, "test.basalt");
    EXPECT_EQ(struct_def.struct_name, "S");
    ASSERT_EQ(struct_def.fields.size(), 1);
    EXPECT_EQ(struct_def.fields.back().field_name, "x");
    ASSERT_EQ(struct_def.template_generics_names.size(), 1);
    EXPECT_EQ(struct_def.template_generics_names.back(), "T");
}