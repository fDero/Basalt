#include <gtest/gtest.h>

#include "core/program_representation.hpp"
#include "core/project_file_structure.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/token.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "../../tests_utilities/compilation_error_assertions.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"


ProjectFileStructure indirectly_recursive_structs_in_same_file({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", { }, {
                    StructDefinition::Field { "b",
                        CustomType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", "main.basalt", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "main.basalt", 1, 1, 1, Token::Type::symbol },
                            CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    },
                    StructDefinition::Field { "slice",
                        SliceType { Token { "$", "main.basalt", 1, 1, 1, Token::Type::symbol },
                            CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    }
});

ProjectFileStructure directly_recursive_structs_in_same_file({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", { }, {
                    StructDefinition::Field { "b",
                        CustomType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", "main.basalt", { }, {
                    StructDefinition::Field { "a",
                        CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            )
        },
        .func_defs = { }
    }
});

TEST(Preprocessing, Non_Direct_Recursive_Two_Struct_Dependency_Is_Ok) {
    ProgramRepresentation program_representation(indirectly_recursive_structs_in_same_file);
    const FileRepresentation& main_dot_basalt = indirectly_recursive_structs_in_same_file.get_files_by_package("testpackage").back();
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(program_representation);
    ASSERT_EQ(A.def_name, "A");
    navigator.visit_struct_definition(A);
}

TEST(Preprocessing, Recursive_Two_Struct_Dependency_Is_Cyclic_Dependency) {
    ProgramRepresentation program_representation(directly_recursive_structs_in_same_file);
    const FileRepresentation& main_dot_basalt = directly_recursive_structs_in_same_file.get_files_by_package("testpackage").back();
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(program_representation);
    ASSERT_EQ(A.def_name, "A");
    EXPECT_COMPILATION_ERROR(ValidationError, "contains itself", {
        navigator.visit_struct_definition(A);
    });
}