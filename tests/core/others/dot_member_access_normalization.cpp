#include <optional>

#include <gtest/gtest.h>

#include "core/program_representation.hpp"
#include "core/project_file_structure.hpp"
#include "core/scope_context.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/token.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/statements.hpp"
#include "language/typesignatures.hpp"
#include "syntax/keywords.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/type_queries.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"


ProjectFileStructure simple_program_with_definition_of_person_struct({
    FileRepresentation {
        .file_metadata = {
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {}
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Person",
                "test.basalt",
                StructDefinitionFactory::no_generics,
                {
                    StructDefinition::Field { "name", TypeSignatureFactory::String },
                    StructDefinition::Field { "age", TypeSignatureFactory::Int }
                }
            )
        },
        .func_defs = {}
    }
});

TEST(Core, Dot_Member_Access_Normalization_Does_Nothing_When_Not_Needed) {
    TypeSignature person_type = CustomType { Token { "Person", "test.basalt", 1, 1, 3, Token::Type::type }, {} };
    VariableDeclaration x_variable_def = VariableDeclaration(
        "x", person_type , std::nullopt,
        Token { var_keyword, "test.basalt", 1, 1, 3, Token::Type::type }   
    );
    Identifier x_variable_expr = Identifier { Token { "x", "test.basalt", 1, 1, 3, Token::Type::text } };
    DotMemberAccess dot_member_access = DotMemberAccess {
        Token { ".", "test.basalt", 1, 1, 3, Token::Type::symbol },
        x_variable_expr,
        "name"
    };
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_def);
    ProgramRepresentation program_representation(simple_program_with_definition_of_person_struct);
    DotMemberAccess normalized = program_representation
        .normalize_dot_member_access(dot_member_access, scope_context);
    std::optional<TypeSignature> normalized_struct_value_type = 
        program_representation.resolve_expression_type(normalized.struct_value, scope_context);
    ASSERT_TRUE(normalized_struct_value_type.has_value());
    EXPECT_TRUE(normalized_struct_value_type.value().is<CustomType>());
}

TEST(Core, Dot_Member_Access_Normalization_Creates_One_DeReference_Operation) {
    TypeSignature person_type = CustomType { Token { "Person", "test.basalt", 1, 1, 3, Token::Type::type }, {} };
    TypeSignature pointer_to_person_type = PointerType { Token { "#", "test.basalt", 1, 1, 3, Token::Type::type }, person_type };
    VariableDeclaration x_variable_def = VariableDeclaration(
        "x", pointer_to_person_type , std::nullopt,
        Token { var_keyword, "test.basalt", 1, 1, 3, Token::Type::type }   
    );
    Identifier x_variable_expr = Identifier { Token { "x", "test.basalt", 1, 1, 3, Token::Type::text } };
    DotMemberAccess dot_member_access = DotMemberAccess {
        Token { ".", "test.basalt", 1, 1, 3, Token::Type::symbol },
        x_variable_expr,
        "name"
    };
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_def);
    ProgramRepresentation program_representation(simple_program_with_definition_of_person_struct);
    DotMemberAccess normalized = program_representation
        .normalize_dot_member_access(dot_member_access, scope_context);
    std::optional<TypeSignature> normalized_struct_value_type = 
        program_representation.resolve_expression_type(normalized.struct_value, scope_context);
    ASSERT_TRUE(normalized_struct_value_type.has_value());
    EXPECT_TRUE(normalized_struct_value_type.value().is<CustomType>());
}

TEST(Core, Dot_Member_Access_Normalization_Creates_The_Necessary_DeReference_Operations) {
    TypeSignature person_type = CustomType { Token { "Person", "test.basalt", 1, 1, 3, Token::Type::type }, {} };
    TypeSignature pointer_to_person_type = PointerType { Token { "#", "test.basalt", 1, 1, 3, Token::Type::type }, person_type };
    TypeSignature pointer_to_pointer_to_person_type = PointerType { Token { "#", "test.basalt", 1, 1, 3, Token::Type::type }, pointer_to_person_type };
    VariableDeclaration x_variable_def = VariableDeclaration(
        "x", pointer_to_pointer_to_person_type , std::nullopt,
        Token { var_keyword, "test.basalt", 1, 1, 3, Token::Type::type }   
    );
    Identifier x_variable_expr = Identifier { Token { "x", "test.basalt", 1, 1, 3, Token::Type::text } };
    DotMemberAccess dot_member_access = DotMemberAccess {
        Token { ".", "test.basalt", 1, 1, 3, Token::Type::symbol },
        x_variable_expr,
        "name"
    };
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_def);
    ProgramRepresentation program_representation(simple_program_with_definition_of_person_struct);
    DotMemberAccess normalized = program_representation
        .normalize_dot_member_access(dot_member_access, scope_context);
    std::optional<TypeSignature> normalized_struct_value_type = 
        program_representation.resolve_expression_type(normalized.struct_value, scope_context);
    ASSERT_TRUE(normalized_struct_value_type.has_value());
    EXPECT_TRUE(normalized_struct_value_type.value().is<CustomType>());
}