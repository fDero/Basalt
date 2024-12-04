 
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure single_file_project_with_multiple_fields_of_same_type({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", StructDefinitionFactory::no_generics, StructDefinitionFactory::no_fields
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", "main.basalt", StructDefinitionFactory::no_generics, {
                    StructDefinition::Field { "a1",
                        CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    },
                    StructDefinition::Field { "a2",
                        CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            },
        .func_defs = { }
    }
});

ProjectFileStructure single_file_project_with_multiple_fields_of_same_type_but_different_concrete_type_parameters({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", StructDefinitionFactory::no_generics, {
                    StructDefinition::Field { "a1",
                        CustomType { Token { "Wrapper", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            TypeSignatureFactory::Int
                        } } 
                    },
                    StructDefinition::Field { "a2",
                        CustomType { Token { "Wrapper", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} }
                        } } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "Wrapper", "main.basalt", { "T" }, {
                    StructDefinition::Field { "wrapped",
                        TypeSignatureFactory::T
                    }
                }
            ),
        },
        .func_defs = { }
    }
});

TEST(Preprocessing, Dependency_Navigation_Works_Fine_With_Multiple_Fields_Of_Same_Type) {
    ProgramRepresentation program_representation(single_file_project_with_multiple_fields_of_same_type);
    const FileRepresentation& main_dot_basalt = single_file_project_with_multiple_fields_of_same_type.get_files_by_package("testpackage").front();
    const StructDefinition& B = main_dot_basalt.type_defs.back().get<StructDefinition>();
    TypeDependencyNavigator navigator(program_representation);
    ASSERT_EQ(B.def_name, "B");
    navigator.visit_struct_definition(B);
}

TEST(Preprocessing, Dependency_Navigation_Works_Fine_With_Multiple_Fields_Of_Different_Instantiations_Of_Same_Generic_Type) {
    ProgramRepresentation program_representation(single_file_project_with_multiple_fields_of_same_type_but_different_concrete_type_parameters);
    const FileRepresentation& main_dot_basalt = single_file_project_with_multiple_fields_of_same_type_but_different_concrete_type_parameters.get_files_by_package("testpackage").front();
    const StructDefinition& A = main_dot_basalt.type_defs.front().get<StructDefinition>();
    TypeDependencyNavigator navigator(program_representation);
    ASSERT_EQ(A.def_name, "A");
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}