
#include <gtest/gtest.h>
#include "syntax/keywords.hpp"
#include "preprocessing/immutability_deducer.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessing, Immutability_Deducer_Says_Const_Identifier_Is_Immutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    ConstDeclaration const_declaration { 
        identifier.name, 
        TypeSignatureFactory::Int, 
        IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
        Token { const_keyword, "test.basalt", 1, 1, 1, Token::Type::const_keyword }
    };
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext scope_context;
    scope_context.store_local_constant(const_declaration);
    ImmutabilityDeducer immutability_deducer(scope_context, empty_program);
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(identifier));
}

TEST(Preprocessing, Immutability_Deducer_Says_Var_Identifier_Is_NON_Immutable_Hence_Mutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    VariableDeclaration var_declaration { 
        identifier.name, 
        TypeSignatureFactory::Int, 
        IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
        Token { var_keyword, "test.basalt", 1, 1, 1, Token::Type::var_keyword }
    };
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext scope_context;
    scope_context.store_local_variable(var_declaration);
    ImmutabilityDeducer immutability_deducer(scope_context, empty_program);
    EXPECT_FALSE(immutability_deducer.is_expression_immutable(identifier));
}