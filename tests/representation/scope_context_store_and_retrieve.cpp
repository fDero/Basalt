
#include <gtest/gtest.h>
#include "toolchain/representation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"
#include "../tests_utilities/type_queries.hpp"

VariableDeclaration x_variable_of_type_int = VariableDeclaration(
    "x", TypeSignatureFactory::Int, std::nullopt,
    Token { "Int", "test.basalt", 1, 1, 3, Token::Type::type }   
);

VariableDeclaration y_variable_of_type_int = VariableDeclaration(
    "y", TypeSignatureFactory::Int, std::nullopt,
    Token { "Int", "test.basalt", 1, 1, 3, Token::Type::type }   
);

VariableDeclaration z_variable_of_type_int = VariableDeclaration(
    "z", TypeSignatureFactory::Int, std::nullopt,
    Token { "Int", "test.basalt", 1, 1, 3, Token::Type::type }   
);

ConstDeclaration a_const_of_type_int = ConstDeclaration(
    "a", TypeSignatureFactory::Int, 
    IntLiteral { Token { "13", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
    Token { "Int", "test.basalt", 1, 1, 3, Token::Type::type }   
);

ConstDeclaration b_const_of_type_int = ConstDeclaration(
    "b", TypeSignatureFactory::Int, 
    IntLiteral { Token { "13", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
    Token { "Int", "test.basalt", 1, 1, 3, Token::Type::type }   
);

ConstDeclaration c_const_of_type_int = ConstDeclaration(
    "c", TypeSignatureFactory::Int, 
    IntLiteral { Token { "13", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
    Token { "Int", "test.basalt", 1, 1, 3, Token::Type::type }   
);

TEST(Representation, Scope_Context_Store_A_Bunch_Of_Local_Objects) {
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_of_type_int);
    scope_context.store_local_variable(y_variable_of_type_int);
    scope_context.store_local_variable(z_variable_of_type_int);
    ASSERT_TRUE(scope_context.contains("x"));
    ASSERT_TRUE(scope_context.contains("y"));
    ASSERT_TRUE(scope_context.contains("z"));
    ASSERT_FALSE(scope_context.contains("a"));
    ASSERT_FALSE(scope_context.contains("b"));
    ASSERT_FALSE(scope_context.contains("c"));
}

TEST(Representation, Scope_Context_Store_A_Bunch_Of_Local_Objects_And_Check_Theri_Types) {
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_of_type_int);
    scope_context.store_local_variable(y_variable_of_type_int);
    EXPECT_TRUE(is_int(scope_context.get_local_object_type("x")));
    EXPECT_TRUE(is_int(scope_context.get_local_mutable_object_type("y")));
}

TEST(Representation, Scope_Context_Store_A_Bunch_Of_Local_Objects_And_Check_Their_Types_One_Is_Constant) {
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_of_type_int);
    scope_context.store_local_constant(a_const_of_type_int);
    EXPECT_TRUE(is_int(scope_context.get_local_object_type("a")));
    EXPECT_TRUE(is_int(scope_context.get_local_mutable_object_type("x")));
}

TEST(Representation, Scope_Context_Contants_Are_Not_Mutable) {
    ScopeContext scope_context;
    scope_context.store_local_constant(a_const_of_type_int);
    EXPECT_TRUE(is_int(scope_context.get_local_object_type("a")));
    EXPECT_ANY_THROW({
        std::ignore = scope_context.get_local_mutable_object_type("a");
    });
}
