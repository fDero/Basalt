
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/functions.hpp"

TEST(Parsing, Simple_Identifiers) {
    std::vector<Token> tokens = {
        { "x", "test.basalt", 1, 1, 1, Token::Type::text },
        { "counter", "test.basalt", 1, 2, 2, Token::Type::text },
        { "variablename", "test.basalt", 1, 3, 8, Token::Type::text }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    for (const auto& identifier : tokens) {
        Expression expr = parser.parse_identifier();
        ASSERT_TRUE(expr.is<Identifier>());
        EXPECT_EQ(expr.get<Identifier>().name, identifier.sourcetext);
    }
}