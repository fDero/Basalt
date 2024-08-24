
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Recursive_Two_Struct_Dependency_Is_Cyclic_Dependency_Even_When_One_Is_Generic) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", { "T" }, {
                    StructDefinition::Field { "b",
                        CustomType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", "main.basalt", { }, {
                    StructDefinition::Field { "a",
                        CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            PrimitiveType { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type } }
                        } }
                    }
                }
            )
        },
        .func_defs = { }
    };
    
    ProjectFileStructure project_file_structure;
    project_file_structure.store_file_representation(main_dot_basalt);
    TypeDefinitionsRegister type_register(project_file_structure);
    TypeDependencyNavigator navigator(type_register);

    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}

TEST(Preprocessor, Type_Dependency_Navigation_Works_Fine_On_Instantiated_Non_Recursive_Generic_Structs) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "Wrapper", "main.basalt", { "T" }, {
                    StructDefinition::Field { "wrapped_object",
                        TemplateType { Token { "T", "main.basalt", 1, 1, 1, Token::Type::type } } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", "main.basalt", { }, {
                    StructDefinition::Field { "wrapper",
                        CustomType { Token { "Wrapper", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            PrimitiveType { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type } }
                        } }
                    }
                }
            )
        },
        .func_defs = { }
    };
    
    ProjectFileStructure project_file_structure;
    project_file_structure.store_file_representation(main_dot_basalt);
    TypeDefinitionsRegister type_register(project_file_structure);
    TypeDependencyNavigator navigator(type_register);
    
    const StructDefinition& B = main_dot_basalt.type_defs[1].get<StructDefinition>();
    navigator.visit_struct_definition(B);
}

TEST(Preprocessor, Type_Dependency_Spots_Cyclic_Dependency_Even_On_Generic_Structs_Instantiation) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "Wrapper", "main.basalt", { "T" }, {
                    StructDefinition::Field { "wrapped_object",
                        TemplateType { Token { "T", "main.basalt", 1, 1, 1, Token::Type::type } } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", "main.basalt", { }, {
                    StructDefinition::Field { "wrapper",
                        CustomType { Token { "Wrapper", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            CustomType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} }
                        } }
                    }
                }
            )
        },
        .func_defs = { }
    };
    
    ProjectFileStructure project_file_structure;
    project_file_structure.store_file_representation(main_dot_basalt);
    TypeDefinitionsRegister type_register(project_file_structure);
    TypeDependencyNavigator navigator(type_register);
    
    const StructDefinition& B = main_dot_basalt.type_defs[1].get<StructDefinition>();
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(B);
    });
}