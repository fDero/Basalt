
#include <gtest/gtest.h>
#include "preprocessing/immutability_checker.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessing, Immutability_Checker_Says_Const_Identifier_Is_Immutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    ConstDeclaration const_declaration { 
        identifier.name, 
        TypeSignatureFactory::Int, 
        IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
        Token { "const", "test.basalt", 1, 1, 1, Token::Type::const_keyword }
    };
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext scope_context;
    scope_context.store_local_constant(const_declaration);
    ImmutabilityChecker immutability_checker(scope_context, empty_program);
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(identifier));
}

TEST(Preprocessing, Immutability_Checker_Says_Var_Identifier_Is_NON_Immutable_Hence_Mutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    VariableDeclaration var_declaration { 
        identifier.name, 
        TypeSignatureFactory::Int, 
        IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
        Token { "var", "test.basalt", 1, 1, 1, Token::Type::var_keyword }
    };
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext scope_context;
    scope_context.store_local_variable(var_declaration);
    ImmutabilityChecker immutability_checker(scope_context, empty_program);
    EXPECT_FALSE(immutability_checker.is_strictly_immutable_expression(identifier));
}