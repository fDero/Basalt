
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Union_Generic_Either_Definition) {
    std::vector<Token> tokens = {
        { union_keyword, "test.basalt",  1,  1,  1,   Token::Type::struct_keyword },
        { "Either",      "test.basalt",  1,  2,  7,   Token::Type::type           },
        { "<",           "test.basalt",  1,  3,  8,   Token::Type::symbol         },
        { "T",           "test.basalt",  1,  2,  9,   Token::Type::type           },
        { ",",           "test.basalt",  1,  2,  10,  Token::Type::symbol         },
        { "U",           "test.basalt",  1,  2,  11,  Token::Type::type           },
        { ">",           "test.basalt",  1,  4,  12,  Token::Type::symbol         },
        { "=",           "test.basalt",  1,  3,  13,  Token::Type::symbol         },
        { "T",           "test.basalt",  1,  4,  14,  Token::Type::type           },
        { "|",           "test.basalt",  1,  3,  13,  Token::Type::symbol         },
        { "U",           "test.basalt",  1,  4,  14,  Token::Type::type           },
        { ";",           "test.basalt",  1,  3,  13,  Token::Type::symbol         }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    UnionDefinition union_def = parser.parse_union_definition();
    EXPECT_EQ(union_def.types.size(), 2);
    EXPECT_EQ(union_def.template_generics_names.size(), 2);
}