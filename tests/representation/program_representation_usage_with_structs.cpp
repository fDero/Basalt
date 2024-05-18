
#include <gtest/gtest.h>
#include "toolchain/representation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(representation, Retrieve_StructDefinition_From_TypeSignature_In_The_Same_File) {
    
    Filerepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    TypeDefinition type_def =  program.retrieve_type_definition(
        BaseType { Token { "MyStruct", "test.basalt", 1, 1, 1, Token::Type::type }, {} }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.struct_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}

TEST(representation, Retrieve_StructDefinition_From_TypeSignature_In_Different_Package_That_Was_Imported) {

    Filerepresentation imported_dot_basalt {
        .file_metadata = { 
            .filename = "imported.basalt",
            .packagename = "importedpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    Filerepresentation main_dot_basalt {
        .file_metadata = { 
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { "importedpackage" },
        },
        .type_defs = {},
        .func_defs = {}
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(main_dot_basalt);
    program.store_definitions_from_file(imported_dot_basalt);

    TypeDefinition type_def =  program.retrieve_type_definition(
        BaseType { Token { "MyStruct", "main.basalt", 1, 1, 1, Token::Type::type }, {} }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.struct_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}

TEST(representation, Retrieve_StructDefinition_From_TypeSignature_In_Different_Files_Of_The_Same_Package) {

    Filerepresentation imported_dot_basalt {
        .file_metadata = { 
            .filename = "imported.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    Filerepresentation main_dot_basalt {
        .file_metadata = { 
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports =  {},
        },
        .type_defs = {},
        .func_defs = {}
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(main_dot_basalt);
    program.store_definitions_from_file(imported_dot_basalt);

    TypeDefinition type_def =  program.retrieve_type_definition(
        BaseType { Token { "MyStruct", "main.basalt", 1, 1, 1, Token::Type::type }, {} }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.struct_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}


TEST(representation, Retrieve_StructDefinition_From_TypeSignature_In_Different_Packages_By_Direct_Path) {

    Filerepresentation other_dot_basalt {
        .file_metadata = { 
            .filename = "other.basalt",
            .packagename = "otherpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "MyStruct", 
                StructDefinitionFactory::no_generics, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    Filerepresentation test_dot_basalt {
        .file_metadata = { 
            .filename = "main.basalt",
            .packagename = "mainpackage",
            .imports =  {},
        },
        .type_defs = {},
        .func_defs = {}
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(test_dot_basalt);
    program.store_definitions_from_file(other_dot_basalt);

    BaseType type_signature = { Token { "MyStruct", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    type_signature.package_prefix = "otherpackage";
    TypeDefinition type_def =  program.retrieve_type_definition(type_signature);

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.struct_name, "MyStruct");
    EXPECT_TRUE(struct_def.fields.empty());
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}