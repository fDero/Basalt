
#include <gtest/gtest.h>
#include "typesystem/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure directly_self_recursive_struct_definition_via_inline_union({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", { }, {
                    StructDefinition::Field { "int_or_a",
                        InlineUnion { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            CustomType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} },
                            PrimitiveType { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type } },
                        } }
                    }
                }
            ),
        },
        .func_defs = { }
    }
});

ProjectFileStructure simple_struct_definition_with_harmless_inline_union({
    FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", "main.basalt", { }, {
                    StructDefinition::Field { "int_or_a",
                        InlineUnion { Token { "Float", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            PrimitiveType { Token { "Float", "main.basalt", 1, 1, 1, Token::Type::type } },
                            PrimitiveType { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type } },
                        } }
                    }
                }
            ),
        },
        .func_defs = { }
    }
});

TEST(Preprocessor, Recursive_Struct_Dependency_Via_Inline_Union_Is_Cyclic_Dependency) {
    TypeDefinitionsRegister type_register(directly_self_recursive_struct_definition_via_inline_union);
    const FileRepresentation& main_dot_basalt = directly_self_recursive_struct_definition_via_inline_union.get_files_by_package("testpackage").front();
    const StructDefinition& A = main_dot_basalt.type_defs.front().get<StructDefinition>();
    TypeDependencyNavigator navigator(type_register);
    ASSERT_EQ(A.def_name, "A");
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}

TEST(Preprocessor, Simple_Struct_Definition_With_Harmless_Inline_Union_Is_Ok) {
    TypeDefinitionsRegister type_register(simple_struct_definition_with_harmless_inline_union);
    const FileRepresentation& main_dot_basalt = simple_struct_definition_with_harmless_inline_union.get_files_by_package("testpackage").front();
    const StructDefinition& A = main_dot_basalt.type_defs.front().get<StructDefinition>();
    TypeDependencyNavigator navigator(type_register);
    ASSERT_EQ(A.def_name, "A");
    navigator.visit_struct_definition(A);
}