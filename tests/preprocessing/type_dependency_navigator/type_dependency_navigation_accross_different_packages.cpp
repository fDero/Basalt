
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure indirectly_recursive_structs_in_different_files_of_different_packages({
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "a.basalt", { }, {
                    StructDefinition::Field { "b",
                        PrimitiveType { Token { "B", "a.basalt", 1, 1, 1, Token::Type::type } } 
                    }
                }
            )
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { "apackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "B", "b.basalt", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "b.basalt", 1, 1, 1, Token::Type::symbol },
                            CustomType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    },
                    StructDefinition::Field { "slice",
                        SliceType { Token { "$", "b.basalt", 1, 1, 1, Token::Type::symbol },
                            CustomType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    }
});

ProjectFileStructure directly_recursive_structs_in_different_files_of_different_packages({
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "a.basalt", { }, {
                    StructDefinition::Field { "b",
                        CustomType { Token { "B", "a.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            )
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { "apackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "B", "b.basalt", { }, {
                    StructDefinition::Field { "a",
                        CustomType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            )
        },
        .func_defs = { }
    }
});

TEST(Preprocessing, Recursive_Two_Struct_Dependency_With_Pointer_Accross_Different_Packages_With_Recursive_Imports_Without_Allucinating_Errors) {
    ProgramRepresentation program_representation(indirectly_recursive_structs_in_different_files_of_different_packages);
    const FileRepresentation file_with_struct_A = indirectly_recursive_structs_in_different_files_of_different_packages.get_files_by_package("apackage").front();
    const StructDefinition& A = file_with_struct_A.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(program_representation);
    ASSERT_EQ(A.def_name, "A");
    navigator.visit_struct_definition(A);
}

TEST(Preprocessing, Recursive_Two_Struct_Dependency_With_Pointer_Accross_Different_Packages_With_Recursive_Imports_Spotting_Real_Errors) {
    ProgramRepresentation program_representation(directly_recursive_structs_in_different_files_of_different_packages);
    const FileRepresentation file_with_struct_A = directly_recursive_structs_in_different_files_of_different_packages.get_files_by_package("apackage").front();
    const StructDefinition& A = file_with_struct_A.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(program_representation);
    ASSERT_EQ(A.def_name, "A");
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}
