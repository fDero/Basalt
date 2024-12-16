
#include <gtest/gtest.h>
#include "preprocessing/bond_inspector.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Bond_Inspector_On_Struct_No_Bond) {
    ProjectFileStructure example_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntWrapper", "test.basalt", {}, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::Int }
                    }                
                )
            },
            .func_defs = { }
        }
    });
    ProgramRepresentation example_program(example_project);
    ScopeContext example_context;
    BondInspector bond_inspector(example_context, example_program);
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bond(
        CustomType {
            Token { "IntWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
            {}
        }
    ));
}

TEST(Preprocessing, Bond_Inspector_On_Struct_With_Bond) {
    ProjectFileStructure example_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntPtrWrapper", "test.basalt", {}, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::PointerToInt }
                    }                
                )
            },
            .func_defs = { }
        }
    });
    ProgramRepresentation example_program(example_project);
    ScopeContext example_context;
    BondInspector bond_inspector(example_context, example_program);
    EXPECT_TRUE(bond_inspector.does_the_type_of_this_expr_imply_a_bond(
        CustomType {
            Token { "IntPtrWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
            {}
        }
    ));
}

TEST(Preprocessing, Bond_Inspector_On_Generic_Struct_No_Bond) {
    ProjectFileStructure example_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "Wrapper", "test.basalt", { "T" }, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::T }
                    }                
                )
            },
            .func_defs = { }
        }
    });
    ProgramRepresentation example_program(example_project);
    ScopeContext example_context;
    BondInspector bond_inspector(example_context, example_program);
    EXPECT_FALSE(bond_inspector.does_the_type_of_this_expr_imply_a_bond(
        CustomType {
            Token { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type },
            {
                TypeSignatureFactory::Int
            }
        }
    ));
}


TEST(Preprocessing, Bond_Inspector_On_Generic_Struct_With_Bond) {
    ProjectFileStructure example_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "Wrapper", "test.basalt", { "T" }, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::T }
                    }                
                )
            },
            .func_defs = { }
        }
    });
    ProgramRepresentation example_program(example_project);
    ScopeContext example_context;
    BondInspector bond_inspector(example_context, example_program);
    EXPECT_TRUE(bond_inspector.does_the_type_of_this_expr_imply_a_bond(
        CustomType {
            Token { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type },
            {
                TypeSignatureFactory::PointerToInt
            }
        }
    ));
}