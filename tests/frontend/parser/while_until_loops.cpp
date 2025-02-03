
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Inline_While_Loop) {
    std::vector<Token> tokens = {
        { while_keyword, "test.basalt", 1, 1, 1, Token::Type::while_keyword },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "alive",       "test.basalt", 1, 3, 3, Token::Type::text          },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { "think",       "test.basalt", 1, 2, 2, Token::Type::text          },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<WhileLoop>());
    ASSERT_EQ(statement.get<WhileLoop>().loop_body.size(), 1);
    EXPECT_TRUE(statement.get<WhileLoop>().loop_body.back().is<FunctionCall>());
}

TEST(Frontend, Parse_Block_While_Loop_Single_Instruction) {
    std::vector<Token> tokens = {
        { while_keyword,      "test.basalt", 1, 1, 1, Token::Type::while_keyword },
        { "(",                "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "alive",            "test.basalt", 1, 3, 3, Token::Type::text },
        { ")",                "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "{",                "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "think",            "test.basalt", 1, 2, 2, Token::Type::text },
        { "(",                "test.basalt", 1, 2, 2, Token::Type::symbol },
        { ")",                "test.basalt", 1, 4, 4, Token::Type::symbol },
        { ";",                "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "}",                "test.basalt", 1, 2, 2, Token::Type::symbol },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<WhileLoop>());
    ASSERT_EQ(statement.get<WhileLoop>().loop_body.size(), 1);
    EXPECT_TRUE(statement.get<WhileLoop>().loop_body.back().is<FunctionCall>());
}

TEST(Frontend, Parse_Block_While_Loop_Multiple_Instructions) {
    std::vector<Token> tokens = {
        { while_keyword, "test.basalt", 1, 1, 1, Token::Type::while_keyword },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "alive",       "test.basalt", 1, 3, 3, Token::Type::text          },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { "{",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { "eat",         "test.basalt", 1, 2, 2, Token::Type::text          },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
        { "code",        "test.basalt", 1, 2, 2, Token::Type::text          },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
        { "sleep",       "test.basalt", 1, 2, 2, Token::Type::text          },
        { "(",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
        { ")",           "test.basalt", 1, 4, 4, Token::Type::symbol        },
        { ";",           "test.basalt", 1, 3, 3, Token::Type::symbol        },
        { "}",           "test.basalt", 1, 2, 2, Token::Type::symbol        },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<WhileLoop>());
    ASSERT_EQ(statement.get<WhileLoop>().loop_body.size(), 3);
    EXPECT_TRUE(statement.get<WhileLoop>().loop_body[0].is<FunctionCall>());
    EXPECT_TRUE(statement.get<WhileLoop>().loop_body[1].is<FunctionCall>());
    EXPECT_TRUE(statement.get<WhileLoop>().loop_body[2].is<FunctionCall>());
}

TEST(Frontend, Parse_Inline_Until_Loop) {
    std::vector<Token> tokens = {
        { until_keyword, "test.basalt", 1, 1, 1, Token::Type::until_keyword },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { "dead", "test.basalt", 1, 3, 3, Token::Type::text                 },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { "think", "test.basalt", 1, 2, 2, Token::Type::text                },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { ";", "test.basalt", 1, 3, 3, Token::Type::symbol                  },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<UntilLoop>());
    ASSERT_EQ(statement.get<UntilLoop>().loop_body.size(), 1);
    EXPECT_TRUE(statement.get<UntilLoop>().loop_body.back().is<FunctionCall>());
}

TEST(Frontend, Parse_Block_Until_Loop_Single_Instruction) {
    std::vector<Token> tokens = {
        { until_keyword, "test.basalt", 1, 1, 1, Token::Type::until_keyword },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { "alive", "test.basalt", 1, 3, 3, Token::Type::text                },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { "{", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { "think", "test.basalt", 1, 2, 2, Token::Type::text                },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { ";", "test.basalt", 1, 3, 3, Token::Type::symbol                  },
        { "}", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<UntilLoop>());
    ASSERT_EQ(statement.get<UntilLoop>().loop_body.size(), 1);
    EXPECT_TRUE(statement.get<UntilLoop>().loop_body.back().is<FunctionCall>());
}

TEST(Frontend, Parse_Block_Until_Loop_Multiple_Instructions) {
    std::vector<Token> tokens = {
        { until_keyword, "test.basalt", 1, 1, 1, Token::Type::until_keyword },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { "alive", "test.basalt", 1, 3, 3, Token::Type::text                },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { "{", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { "eat", "test.basalt", 1, 2, 2, Token::Type::text                  },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { ";", "test.basalt", 1, 3, 3, Token::Type::symbol                  },
        { "code", "test.basalt", 1, 2, 2, Token::Type::text                 },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { ";", "test.basalt", 1, 3, 3, Token::Type::symbol                  },
        { "sleep", "test.basalt", 1, 2, 2, Token::Type::text                },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol                  },
        { ";", "test.basalt", 1, 3, 3, Token::Type::symbol                  },
        { "}", "test.basalt", 1, 2, 2, Token::Type::symbol                  },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<UntilLoop>());
    ASSERT_EQ(statement.get<UntilLoop>().loop_body.size(), 3);
    EXPECT_TRUE(statement.get<UntilLoop>().loop_body[0].is<FunctionCall>());
    EXPECT_TRUE(statement.get<UntilLoop>().loop_body[1].is<FunctionCall>());
    EXPECT_TRUE(statement.get<UntilLoop>().loop_body[2].is<FunctionCall>());
}