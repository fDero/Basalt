
#include <gtest/gtest.h>
#include "toolchain/representation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(representation, Retrieve_Generic_StructDefinition_From_TypeSignature_In_The_Same_File) {
    
    Filerepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Pair", 
                { "T", "U" }, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    TypeDefinition type_def =  program.retrieve_type_definition(
        BaseType { Token { "Pair", "test.basalt", 1, 1, 1, Token::Type::type }, {
            PrimitiveType { Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type } },
            PrimitiveType { Token { "String", "test.basalt", 1, 1, 1, Token::Type::type } },
        } }
    );

    ASSERT_TRUE(type_def.is<StructDefinition>());
    StructDefinition struct_def = type_def.get<StructDefinition>();
    EXPECT_EQ(struct_def.struct_name, "Pair<Int,String>");
    EXPECT_TRUE(struct_def.template_generics_names.empty());
}

TEST(representation, Struct_Overloads_Are_Distinguished_By_Template_Generics_Count) {
    
    Filerepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "Pair", 
                { "T", "U" }, 
                StructDefinitionFactory::no_fields
            ),

            StructDefinitionFactory::make_struct_definition(
                "Pair", 
                { "T" }, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {}
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    TypeDefinition heterogeneous_pair =  program.retrieve_type_definition(
        BaseType { Token { "Pair", "test.basalt", 1, 1, 1, Token::Type::type }, {
            PrimitiveType { Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type } },
            PrimitiveType { Token { "String", "test.basalt", 1, 1, 1, Token::Type::type } },
        } }
    );

    TypeDefinition homogeneous_pair =  program.retrieve_type_definition(
        BaseType { Token { "Pair", "test.basalt", 1, 1, 1, Token::Type::type }, {
            PrimitiveType { Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type } }
        } }
    );

    ASSERT_TRUE(heterogeneous_pair.is<StructDefinition>());
    StructDefinition heterogeneous_pair_def = heterogeneous_pair.get<StructDefinition>();
    EXPECT_EQ(heterogeneous_pair_def.struct_name, "Pair<Int,String>");
    EXPECT_TRUE(heterogeneous_pair_def.template_generics_names.empty());

    ASSERT_TRUE(homogeneous_pair.is<StructDefinition>());
    StructDefinition homogeneous_pair_def = homogeneous_pair.get<StructDefinition>();
    EXPECT_EQ(homogeneous_pair_def.struct_name, "Pair<Int>");
    EXPECT_TRUE(homogeneous_pair_def.template_generics_names.empty());
}