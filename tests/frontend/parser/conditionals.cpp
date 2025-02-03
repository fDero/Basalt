
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_If_Statement_Inline_Then_No_Else) {
    std::vector<Token> tokens = {
        { if_keyword,    "test.basalt", 1, 1, 1, Token::Type::if_keyword    },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "ready",       "test.basalt", 1, 3, 3, Token::Type::text          },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { break_keyword, "test.basalt", 1, 2, 2, Token::Type::break_keyword },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Conditional>());
    ASSERT_TRUE(statement.get<Conditional>().else_branch.empty());
    ASSERT_EQ(statement.get<Conditional>().then_branch.size(), 1);
    EXPECT_TRUE(statement.get<Conditional>().then_branch.back().is<Break>());
    EXPECT_TRUE(statement.get<Conditional>().condition.is<Identifier>());
}

TEST(Frontend, Parse_If_Statement_Inline_Then_Inline_Else) {
    std::vector<Token> tokens = {
        { if_keyword,       "test.basalt", 1, 1, 1, Token::Type::if_keyword       },
        { "(",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { "ready",          "test.basalt", 1, 3, 3, Token::Type::text             },
        { ")",              "test.basalt", 1, 4, 4, Token::Type::symbol           },
        { break_keyword,    "test.basalt", 1, 2, 2, Token::Type::break_keyword    },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { else_keyword,     "test.basalt", 1, 3, 3, Token::Type::else_keyword     },
        { continue_keyword, "test.basalt", 1, 2, 2, Token::Type::continue_keyword },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
    };
    Parser parser = Parser(TokenizedFile{ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Conditional>());
    ASSERT_EQ(statement.get<Conditional>().then_branch.size(), 1);
    ASSERT_EQ(statement.get<Conditional>().else_branch.size(), 1);
    EXPECT_TRUE(statement.get<Conditional>().then_branch.back().is<Break>());
    EXPECT_TRUE(statement.get<Conditional>().else_branch.back().is<Continue>());
    EXPECT_TRUE(statement.get<Conditional>().condition.is<Identifier>());
}

TEST(Frontend, Parse_If_Statement_Block_Then_Block_Else) {
    std::vector<Token> tokens = {
        { if_keyword,       "test.basalt", 1, 1, 1, Token::Type::if_keyword       },
        { "(",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { "ready",          "test.basalt", 1, 3, 3, Token::Type::text             },
        { ")",              "test.basalt", 1, 4, 4, Token::Type::symbol           },
        { "{",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { break_keyword,    "test.basalt", 1, 2, 2, Token::Type::break_keyword    },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { "}",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { else_keyword,     "test.basalt", 1, 3, 3, Token::Type::else_keyword     },
        { "{",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { continue_keyword, "test.basalt", 1, 2, 2, Token::Type::continue_keyword },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { "}",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Conditional>());
    ASSERT_EQ(statement.get<Conditional>().then_branch.size(), 1);
    ASSERT_EQ(statement.get<Conditional>().else_branch.size(), 1);
    EXPECT_TRUE(statement.get<Conditional>().then_branch.back().is<Break>());
    EXPECT_TRUE(statement.get<Conditional>().else_branch.back().is<Continue>());
    EXPECT_TRUE(statement.get<Conditional>().condition.is<Identifier>());
}

TEST(Frontend, Parse_If_Statement_Block_Then_Block_Else_More_Then_One_Statement_In_Body) {
    std::vector<Token> tokens = {
        { if_keyword,       "test.basalt", 1, 1, 1, Token::Type::if_keyword       },
        { "(",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { "ready",          "test.basalt", 1, 3, 3, Token::Type::text             },
        { ")",              "test.basalt", 1, 4, 4, Token::Type::symbol           },
        { "{",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { break_keyword,    "test.basalt", 1, 2, 2, Token::Type::break_keyword    },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { break_keyword,    "test.basalt", 1, 2, 2, Token::Type::break_keyword    },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { "}",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { else_keyword,     "test.basalt", 1, 3, 3, Token::Type::else_keyword     },
        { "{",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
        { continue_keyword, "test.basalt", 1, 2, 2, Token::Type::continue_keyword },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { continue_keyword, "test.basalt", 1, 2, 2, Token::Type::continue_keyword },
        { ";",              "test.basalt", 1, 3, 3, Token::Type::symbol           },
        { "}",              "test.basalt", 1, 2, 2, Token::Type::symbol           },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Conditional>());
    ASSERT_EQ(statement.get<Conditional>().then_branch.size(), 2);
    ASSERT_EQ(statement.get<Conditional>().else_branch.size(), 2);
    EXPECT_TRUE(statement.get<Conditional>().then_branch.back().is<Break>());
    EXPECT_TRUE(statement.get<Conditional>().else_branch.back().is<Continue>());
    EXPECT_TRUE(statement.get<Conditional>().then_branch.front().is<Break>());
    EXPECT_TRUE(statement.get<Conditional>().else_branch.front().is<Continue>());
    EXPECT_TRUE(statement.get<Conditional>().condition.is<Identifier>());
}

TEST(Frontend, Parse_If_Statement_Inline_Then_No_Else_Condition_Is_Function_Call) {
    std::vector<Token> tokens = {
        { if_keyword,    "test.basalt", 1, 1, 1, Token::Type::if_keyword    },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "ready",       "test.basalt", 1, 3, 3, Token::Type::text          },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { break_keyword, "test.basalt", 1, 2, 2, Token::Type::break_keyword },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Conditional>());
    ASSERT_TRUE(statement.get<Conditional>().else_branch.empty());
    ASSERT_EQ(statement.get<Conditional>().then_branch.size(), 1);
    EXPECT_TRUE(statement.get<Conditional>().then_branch.back().is<Break>());
    EXPECT_TRUE(statement.get<Conditional>().condition.is<FunctionCall>());
}

TEST(Frontend, Parse_If_Statement_Inline_Then_No_Else_Condition_Is_Binary_Operator) {
    std::vector<Token> tokens = {
        { if_keyword,    "test.basalt", 1, 1, 1, Token::Type::if_keyword    },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "ready",       "test.basalt", 1, 3, 3, Token::Type::text          },
        { "||",          "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "flag",        "test.basalt", 1, 4, 4, Token::Type::text          },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { break_keyword, "test.basalt", 1, 2, 2, Token::Type::break_keyword },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Conditional>());
    ASSERT_TRUE(statement.get<Conditional>().else_branch.empty());
    ASSERT_TRUE(statement.get<Conditional>().condition.is<BinaryOperator>());
    ASSERT_EQ(statement.get<Conditional>().then_branch.size(), 1);
    EXPECT_TRUE(statement.get<Conditional>().then_branch.back().is<Break>());
    EXPECT_EQ(statement.get<Conditional>().condition.get<BinaryOperator>().operator_text, "||");
    ASSERT_TRUE(statement.get<Conditional>().condition.get<BinaryOperator>().left_operand.is<Identifier>());
    ASSERT_TRUE(statement.get<Conditional>().condition.get<BinaryOperator>().left_operand.is<Identifier>());   
    ASSERT_EQ(statement.get<Conditional>().condition.get<BinaryOperator>().left_operand.get<Identifier>().name, "ready");
    EXPECT_EQ(statement.get<Conditional>().condition.get<BinaryOperator>().right_operand.get<Identifier>().name, "flag");   
}