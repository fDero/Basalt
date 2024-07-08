
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure indirectly_recursive_structs_in_same_file({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        CustomType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
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
                "A", { }, {
                    StructDefinition::Field { "b",
                        CustomType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "a",
                        CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            )
        },
        .func_defs = { }
    }
});

TEST(Preprocessor, Non_Direct_Recursive_Two_Struct_Dependency_Is_Ok) {
    TypeDefinitionsRegister type_register(indirectly_recursive_structs_in_same_file);
    const FileRepresentation& main_dot_basalt = indirectly_recursive_structs_in_same_file.get_files_by_package("testpackage").back();
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(type_register);
    ASSERT_EQ(A.struct_name, "A");
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Recursive_Two_Struct_Dependency_Is_Cyclic_Dependency) {
    TypeDefinitionsRegister type_register(directly_recursive_structs_in_same_file);
    const FileRepresentation& main_dot_basalt = directly_recursive_structs_in_same_file.get_files_by_package("testpackage").back();
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(type_register);
    ASSERT_EQ(A.struct_name, "A");
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}