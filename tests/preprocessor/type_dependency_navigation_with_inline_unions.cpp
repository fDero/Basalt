
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Recursive_Struct_Dependency_Via_Inline_Union_Is_Cyclic_Dependency) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
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
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(main_dot_basalt);
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(representation);
    EXPECT_ANY_THROW({
        navigator.visit_struct_definition(A);
    });
}

TEST(Preprocessor, No_Cyclic_Dependency_With_Simple_Inlien_Union) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "int_or_float",
                        InlineUnion { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {
                            PrimitiveType { Token { "Float", "main.basalt", 1, 1, 1, Token::Type::type },},
                            PrimitiveType { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type } },
                        } }
                    }
                }
            ),
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(main_dot_basalt);
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(representation);
    navigator.visit_struct_definition(A);
}