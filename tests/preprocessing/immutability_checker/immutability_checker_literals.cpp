
#include <gtest/gtest.h>
#include "preprocessing/immutability_checker.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessing, Immutability_Checker_Says_Int_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityChecker immutability_checker(empty_scope_context, empty_program);
    IntLiteral int_literal { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(int_literal));
}

TEST(Preprocessing, Immutability_Checker_Says_Bool_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityChecker immutability_checker(empty_scope_context, empty_program);
    BoolLiteral bool_literal { Token { "true", "test.basalt", 1, 1, 1, Token::Type::boolean_literal } };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(bool_literal));
}

TEST(Preprocessing, Immutability_Checker_Says_Char_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityChecker immutability_checker(empty_scope_context, empty_program);
    CharLiteral char_literal { Token { "'c'", "test.basalt", 1, 1, 1, Token::Type::character_literal } };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(char_literal));
}

TEST(Preprocessing, Immutability_Checker_Says_Float_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityChecker immutability_checker(empty_scope_context, empty_program);
    FloatLiteral float_literal { Token { "15.6", "test.basalt", 1, 1, 1, Token::Type::floating_literal } };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(float_literal));
}

TEST(Preprocessing, Immutability_Checker_Says_String_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityChecker immutability_checker(empty_scope_context, empty_program);
    StringLiteral string_literal { Token { "15.6", "test.basalt", 1, 1, 1, Token::Type::string_literal } };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(string_literal));
}

TEST(Preprocessing, Immutability_Checker_Says_Array_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityChecker immutability_checker(empty_scope_context, empty_program);
    ArrayLiteral array_literal { std::nullopt, TypeSignatureFactory::Int, { 
            IntLiteral { Token { "1", "test.basalt", 1, 2, 2, Token::Type::integer_literal } },
            IntLiteral { Token { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal } } 
        },
        Token { "[", "test.basalt", 1, 1, 1, Token::Type::integer_literal } 
    };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(array_literal));
}