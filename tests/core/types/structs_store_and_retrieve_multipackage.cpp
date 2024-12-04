
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

TEST(Core, Retrieve_StructDefinition_From_TypeSignature_In_The_Same_File) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                "test.basalt",
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(file);
    TypeDefinitionsRegister type_register(project);

    TypeDefinition type_def =  type_register.retrieve_type_definition(
        CustomType { Token { "MyStruct", "test.basalt", 1, 1, 1, Token::Type::type }, {} }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.def_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}

TEST(Core, Retrieve_StructDefinition_From_TypeSignature_In_Different_Package_That_Was_Imported) {

    FileRepresentation imported_dot_basalt {
        .file_metadata = { 
            .filename = "imported.basalt",
            .packagename = "importedpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                "imported.basalt",
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    FileRepresentation main_dot_basalt {
        .file_metadata = { 
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { "importedpackage" },
        },
        .type_defs = {},
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(imported_dot_basalt);
    project.store_file_representation(main_dot_basalt);
    TypeDefinitionsRegister type_register(project);

    TypeDefinition type_def =  type_register.retrieve_type_definition(
        CustomType { Token { "MyStruct", "main.basalt", 1, 1, 1, Token::Type::type }, {} }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.def_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}

TEST(Core, Retrieve_StructDefinition_From_TypeSignature_In_Different_Files_Of_The_Same_Package) {

    FileRepresentation imported_dot_basalt {
        .file_metadata = { 
            .filename = "imported.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                "imported.basalt",
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    FileRepresentation main_dot_basalt {
        .file_metadata = { 
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports =  {},
        },
        .type_defs = {},
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(imported_dot_basalt);
    project.store_file_representation(main_dot_basalt);
    TypeDefinitionsRegister type_register(project);

    TypeDefinition type_def =  type_register.retrieve_type_definition(
        CustomType { Token { "MyStruct", "main.basalt", 1, 1, 1, Token::Type::type }, {} }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.def_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}


TEST(Core, Retrieve_StructDefinition_From_TypeSignature_In_Different_Packages_By_Direct_Path) {

    FileRepresentation other_dot_basalt {
        .file_metadata = { 
            .filename = "other.basalt",
            .packagename = "otherpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                "other.basalt",
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    FileRepresentation test_dot_basalt {
        .file_metadata = { 
            .filename = "main.basalt",
            .packagename = "mainpackage",
            .imports =  {},
        },
        .type_defs = {},
        .func_defs = {}
    };

    ProjectFileStructure project;
    project.store_file_representation(other_dot_basalt);
    project.store_file_representation(test_dot_basalt);
    TypeDefinitionsRegister type_register(project);

    CustomType type_signature = { Token { "MyStruct", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    type_signature.package_prefix = "otherpackage";
    TypeDefinition type_def = type_register.retrieve_type_definition(type_signature);

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.def_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}