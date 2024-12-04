
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

FileRepresentation generic_recursion_problem_file = {
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

FileRepresentation legal_wrapper_use = {
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

FileRepresentation illegal_wrapper_use = {
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

TEST(Preprocessing, Recursive_Two_Struct_Dependency_Is_Cyclic_Dependency_Even_When_One_Is_Generic) {  
    ProjectFileStructure project_file_structure;
    project_file_structure.store_file_representation(generic_recursion_problem_file);
    ProgramRepresentation program_representation(project_file_structure);
    TypeDependencyNavigator navigator(program_representation);
    const StructDefinition& A = generic_recursion_problem_file.type_defs[0].get<StructDefinition>();
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}

TEST(Preprocessing, Type_Dependency_Navigation_Works_Fine_On_Instantiated_Non_Recursive_Generic_Structs) {
    ProjectFileStructure project_file_structure;
    project_file_structure.store_file_representation(legal_wrapper_use);
    ProgramRepresentation program_representation(project_file_structure);
    TypeDependencyNavigator navigator(program_representation);
    const StructDefinition& B = legal_wrapper_use.type_defs[1].get<StructDefinition>();
    navigator.visit_struct_definition(B);
}

TEST(Preprocessing, Type_Dependency_Spots_Cyclic_Dependency_Even_On_Generic_Structs_Instantiation) {    
    ProjectFileStructure project_file_structure;
    project_file_structure.store_file_representation(illegal_wrapper_use);
    ProgramRepresentation program_representation(project_file_structure);
    TypeDependencyNavigator navigator(program_representation);
    const StructDefinition& B = illegal_wrapper_use.type_defs[1].get<StructDefinition>();
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(B);
    });
}