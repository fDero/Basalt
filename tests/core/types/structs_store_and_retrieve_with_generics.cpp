
#include <gtest/gtest.h>
#include "core/type_definitions_register.hpp"
#include "frontend/file_representation.hpp"
#include "core/project_file_structure.hpp"
#include "frontend/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"
#include "../../tests_utilities/type_queries.hpp"

TEST(Core, Retrieve_Generic_StructDefinition_From_TypeSignature_In_The_Same_File) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Pair", 
                "test.basalt",
                { "T", "U" }, 
                {
                    StructDefinition::Field { "first", TypeSignatureFactory::T },
                    StructDefinition::Field { "second", TypeSignatureFactory::U }
                }
            )
        },
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(file);
    TypeDefinitionsRegister type_register(project);

    TypeDefinition type_def =  type_register.retrieve_type_definition(
        CustomType { Token { "Pair", "test.basalt", 1, 1, 1, Token::Type::type }, {
            PrimitiveType { Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type } },
            PrimitiveType { Token { "String", "test.basalt", 1, 1, 1, Token::Type::type } },
        } }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_TRUE(struct_def.template_generics_names.empty());
    ASSERT_EQ(struct_def.fields.size(), 2);
    EXPECT_EQ(struct_def.fields[0].field_name, "first");
    EXPECT_EQ(struct_def.fields[1].field_name, "second");
    EXPECT_TRUE(is_int(struct_def.fields[0].field_type));
    EXPECT_TRUE(is_string(struct_def.fields[1].field_type));
}

TEST(Core, Struct_Overloads_Are_Distinguished_By_Template_Generics_Count) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Pair", 
                "test.basalt",
                { "T", "U" }, 
                {
                    StructDefinition::Field { "first", TypeSignatureFactory::T },
                    StructDefinition::Field { "second", TypeSignatureFactory::U }
                }
            ),

            StructDefinitionFactory::make_struct_definition(
                "Pair", 
                "test.basalt",
                { "T" }, 
                {
                    StructDefinition::Field { "first", TypeSignatureFactory::T },
                    StructDefinition::Field { "second", TypeSignatureFactory::T }
                }
            )
        },
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(file);
    TypeDefinitionsRegister type_register(project);

    TypeDefinition heterogeneous_pair =  type_register.retrieve_type_definition(
        CustomType { Token { "Pair", "test.basalt", 1, 1, 1, Token::Type::type }, {
            PrimitiveType { Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type } },
            PrimitiveType { Token { "String", "test.basalt", 1, 1, 1, Token::Type::type } },
        } }
    );
    TypeDefinition homogeneous_pair =  type_register.retrieve_type_definition(
        CustomType { Token { "Pair", "test.basalt", 1, 1, 1, Token::Type::type }, {
            PrimitiveType { Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type } }
        } }
    );
    ASSERT_TRUE(heterogeneous_pair.is<StructDefinition>());
    StructDefinition heterogeneous_pair_def = heterogeneous_pair.get<StructDefinition>();
    EXPECT_TRUE(heterogeneous_pair_def.template_generics_names.empty());
    ASSERT_TRUE(homogeneous_pair.is<StructDefinition>());
    StructDefinition homogeneous_pair_def = homogeneous_pair.get<StructDefinition>();
    EXPECT_TRUE(homogeneous_pair_def.template_generics_names.empty());

    ASSERT_EQ(heterogeneous_pair_def.fields.size(), 2);
    EXPECT_EQ(heterogeneous_pair_def.fields[0].field_name, "first");
    EXPECT_EQ(heterogeneous_pair_def.fields[1].field_name, "second");
    EXPECT_TRUE(is_int(heterogeneous_pair_def.fields[0].field_type));
    EXPECT_TRUE(is_string(heterogeneous_pair_def.fields[1].field_type));

    ASSERT_EQ(homogeneous_pair_def.fields.size(), 2);
    EXPECT_EQ(homogeneous_pair_def.fields[0].field_name, "first");
    EXPECT_EQ(homogeneous_pair_def.fields[1].field_name, "second");
    EXPECT_TRUE(is_int(homogeneous_pair_def.fields[0].field_type));
    EXPECT_TRUE(is_int(homogeneous_pair_def.fields[1].field_type));
}