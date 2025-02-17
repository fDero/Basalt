
#include <gtest/gtest.h>
#include "preprocessing/immutability_deducer.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessing, Immutability_Deducer_Says_Int_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityDeducer immutability_deducer(empty_scope_context, empty_program);
    IntLiteral int_literal { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } };
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(int_literal));
}

TEST(Preprocessing, Immutability_Deducer_Says_Bool_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityDeducer immutability_deducer(empty_scope_context, empty_program);
    BoolLiteral bool_literal { Token { "true", "test.basalt", 1, 1, 1, Token::Type::boolean_literal } };
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(bool_literal));
}

TEST(Preprocessing, Immutability_Deducer_Says_Char_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityDeducer immutability_deducer(empty_scope_context, empty_program);
    CharLiteral char_literal { Token { "'c'", "test.basalt", 1, 1, 1, Token::Type::character_literal }, 'c' };
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(char_literal));
}

TEST(Preprocessing, Immutability_Deducer_Says_Float_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityDeducer immutability_deducer(empty_scope_context, empty_program);
    FloatLiteral float_literal { Token { "15.6", "test.basalt", 1, 1, 1, Token::Type::floating_literal } };
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(float_literal));
}

TEST(Preprocessing, Immutability_Deducer_Says_String_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityDeducer immutability_deducer(empty_scope_context, empty_program);
    StringLiteral string_literal { Token { "AAA", "test.basalt", 1, 1, 1, Token::Type::string_literal }, "AAA" };
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(string_literal));
}

TEST(Preprocessing, Immutability_Deducer_Says_Array_Literal_Is_Immutable) {
    ProjectFileStructure empty_project;
    ProgramRepresentation empty_program(empty_project);
    ScopeContext empty_scope_context;
    ImmutabilityDeducer immutability_deducer(empty_scope_context, empty_program);
    ArrayLiteral array_literal { std::nullopt, TypeSignatureFactory::Int, { 
            IntLiteral { Token { "1", "test.basalt", 1, 2, 2, Token::Type::integer_literal } },
            IntLiteral { Token { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal } } 
        },
        Token { "[", "test.basalt", 1, 1, 1, Token::Type::integer_literal } 
    };
    EXPECT_TRUE(immutability_deducer.is_expression_immutable(array_literal));
}