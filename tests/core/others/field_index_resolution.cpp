 
#include <gtest/gtest.h>
#include "core/dot_member_access_normalizer.hpp"
#include "core/project_file_structure.hpp"
#include "core/program_representation.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"
#include "../../tests_utilities/type_queries.hpp"

ProjectFileStructure simple_program_with_definition_of_color_struct({
    FileRepresentation {
        .file_metadata = {
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {}
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Color",
                "test.basalt",
                StructDefinitionFactory::no_generics,
                {
                    StructDefinition::Field { "red", TypeSignatureFactory::Int },
                    StructDefinition::Field { "green", TypeSignatureFactory::Int },
                    StructDefinition::Field { "blue", TypeSignatureFactory::Int }
                }
            )
        },
        .func_defs = {}
    }
});

TEST(Core, Field_Index_Resolution_Success_On_First_Field) {
    TypeSignature person_type = CustomType { Token { "Color", "test.basalt", 1, 1, 3, Token::Type::type }, {} };
    VariableDeclaration x_variable_def = VariableDeclaration(
        "x", person_type , std::nullopt,
        Token { "var", "test.basalt", 1, 1, 3, Token::Type::type }   
    );
    Identifier x_variable_expr = Identifier { Token { "x", "test.basalt", 1, 1, 3, Token::Type::text } };
    DotMemberAccess dot_member_access = DotMemberAccess {
        Token { ".", "test.basalt", 1, 1, 3, Token::Type::symbol },
        x_variable_expr,
        "red"
    };
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_def);
    ProgramRepresentation program_representation(simple_program_with_definition_of_color_struct);
    size_t index = program_representation.resolve_field_index(dot_member_access, scope_context);
    EXPECT_EQ(index, 0);
}

TEST(Core, Field_Index_Resolution_Success_On_Second_Field) {
    TypeSignature person_type = CustomType { Token { "Color", "test.basalt", 1, 1, 3, Token::Type::type }, {} };
    VariableDeclaration x_variable_def = VariableDeclaration(
        "x", person_type , std::nullopt,
        Token { "var", "test.basalt", 1, 1, 3, Token::Type::type }   
    );
    Identifier x_variable_expr = Identifier { Token { "x", "test.basalt", 1, 1, 3, Token::Type::text } };
    DotMemberAccess dot_member_access = DotMemberAccess {
        Token { ".", "test.basalt", 1, 1, 3, Token::Type::symbol },
        x_variable_expr,
        "green"
    };
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_def);
    ProgramRepresentation program_representation(simple_program_with_definition_of_color_struct);
    size_t index = program_representation.resolve_field_index(dot_member_access, scope_context);
    EXPECT_EQ(index, 1);
}

TEST(Core, Field_Index_Resolution_Success_On_Third_Field) {
    TypeSignature person_type = CustomType { Token { "Color", "test.basalt", 1, 1, 3, Token::Type::type }, {} };
    VariableDeclaration x_variable_def = VariableDeclaration(
        "x", person_type , std::nullopt,
        Token { "var", "test.basalt", 1, 1, 3, Token::Type::type }   
    );
    Identifier x_variable_expr = Identifier { Token { "x", "test.basalt", 1, 1, 3, Token::Type::text } };
    DotMemberAccess dot_member_access = DotMemberAccess {
        Token { ".", "test.basalt", 1, 1, 3, Token::Type::symbol },
        x_variable_expr,
        "blue"
    };
    ScopeContext scope_context;
    scope_context.store_local_variable(x_variable_def);
    ProgramRepresentation program_representation(simple_program_with_definition_of_color_struct);
    size_t index = program_representation.resolve_field_index(dot_member_access, scope_context);
    EXPECT_EQ(index, 2);
}