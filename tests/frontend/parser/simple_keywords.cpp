
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Return_Nothing) {
    std::vector<Token> tokens = {
        { return_keyword, "test.basalt", 1, 1, 1, Token::Type::return_keyword },
        { ";",            "test.basalt", 1, 4, 4, Token::Type::symbol         },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Return>());
    EXPECT_FALSE(statement.get<Return>().return_value.has_value());
}

TEST(Frontend, Parse_Return_Integer) {
    std::vector<Token> tokens = {
        { return_keyword, "test.basalt", 1, 1, 1, Token::Type::return_keyword  },
        { "6",            "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { ";",            "test.basalt", 1, 4, 4, Token::Type::symbol          },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Return>());
    ASSERT_TRUE(statement.get<Return>().return_value.has_value());
    ASSERT_TRUE(statement.get<Return>().return_value->is<IntLiteral>());
    EXPECT_EQ(statement.get<Return>().return_value->get<IntLiteral>().value, 6);
}

TEST(Frontend, Parse_Return_Expression) {
    std::vector<Token> tokens = {
        { return_keyword, "test.basalt", 1, 1, 1, Token::Type::return_keyword  },
        { "6",            "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { "*",            "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { "7",            "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { ";",            "test.basalt", 1, 4, 4, Token::Type::symbol          },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Return>());
    ASSERT_TRUE(statement.get<Return>().return_value.has_value());
    ASSERT_TRUE(statement.get<Return>().return_value->is<BinaryOperator>());
}