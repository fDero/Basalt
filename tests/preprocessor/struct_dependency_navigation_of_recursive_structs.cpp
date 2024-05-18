
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Recursive_Two_Struct_Dependency_Is_Cyclic_Dependency) {
    
    Filerepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        BaseType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "a",
                        BaseType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            )
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

TEST(Preprocessor, Recursive_Two_Struct_Dependency_With_Pointer) {
    
    Filerepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        BaseType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "main.basalt", 1, 1, 1, Token::Type::symbol },
                            BaseType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(main_dot_basalt);
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(representation);
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Recursive_Two_Struct_Dependency_With_Slices) {
    
    Filerepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        BaseType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "slice",
                        SliceType { Token { "$", "main.basalt", 1, 1, 1, Token::Type::symbol },
                            BaseType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    };
    
    ProgramRepresentation representation;
    representation.store_definitions_from_file(main_dot_basalt);
    const StructDefinition& A = main_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(representation);
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Recursive_Two_Struct_Dependency_With_Array_Is_Cyclic_Dependency) {
    
    Filerepresentation main_dot_basalt = {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        BaseType { Token { "B", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            ),
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "arr",
                        ArrayType { Token { "[", "main.basalt", 1, 1, 1, Token::Type::symbol }, 10,
                            BaseType { Token { "A", "main.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
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