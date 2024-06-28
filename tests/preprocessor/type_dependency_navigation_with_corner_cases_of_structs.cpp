
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Dependency_Navigation_Works_Fine_With_Multiple_Fields_Of_Same_Type) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", StructDefinitionFactory::no_generics, StructDefinitionFactory::no_fields
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", StructDefinitionFactory::no_generics, {
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
    };   
    ProgramRepresentation representation;
    representation.store_definitions_from_file(main_dot_basalt);
    const StructDefinition& B = main_dot_basalt.type_defs[1].get<StructDefinition>();
    TypeDependencyNavigator navigator(representation);
    navigator.visit_struct_definition(B);
}

TEST(Preprocessor, Dependency_Navigation_Works_Fine_With_Multiple_Fields_Of_Different_Instantiations_Of_Same_Generic_Type) {
    
    FileRepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", StructDefinitionFactory::no_generics, {
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
                "Wrapper", { "T" }, {
                    StructDefinition::Field { "wrapped",
                        TypeSignatureFactory::T
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