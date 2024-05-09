
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Recursive_Two_Struct_Dependency_With_Pointer_Accross_Different_Files_Of_The_Same_Package) {
    
    FileRappresentation a_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        BaseType { Token { "B", "a.basalt", 1, 1, 1, Token::Type::type }, {} } 
                    }
                }
            )
        },
        .func_defs = { }
    };

    
    FileRappresentation b_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "b.basalt", 1, 1, 1, Token::Type::symbol },
                            BaseType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    };

    ProgramRappresentation rappresentation;
    rappresentation.store_definitions_from_file(a_dot_basalt);
    rappresentation.store_definitions_from_file(b_dot_basalt);
    const StructDefinition& A = a_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(rappresentation);
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Simple_Two_Struct_Dependency_With_Pointer_Accross_Different_Packages_With_Import) {
    
    FileRappresentation a_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "importedpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        PrimitiveType { Token { "Int", "a.basalt", 1, 1, 1, Token::Type::type } } 
                    }
                }
            )
        },
        .func_defs = { }
    };

    
    FileRappresentation b_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "testpackage",
            .imports = { "importedpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "b.basalt", 1, 1, 1, Token::Type::symbol },
                            BaseType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    };

    ProgramRappresentation rappresentation;
    rappresentation.store_definitions_from_file(a_dot_basalt);
    rappresentation.store_definitions_from_file(b_dot_basalt);
    const StructDefinition& A = a_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(rappresentation);
    navigator.visit_struct_definition(A);
}


TEST(Preprocessor, Recursive_Two_Struct_Dependency_With_Pointer_Accross_Different_Packages_With_Recursive_Imports) {
    
    FileRappresentation a_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        PrimitiveType { Token { "Int", "a.basalt", 1, 1, 1, Token::Type::type } } 
                    }
                }
            )
        },
        .func_defs = { }
    };

    
    FileRappresentation b_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { "apackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "b.basalt", 1, 1, 1, Token::Type::symbol },
                            BaseType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} } 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    };

    ProgramRappresentation rappresentation;
    rappresentation.store_definitions_from_file(a_dot_basalt);
    rappresentation.store_definitions_from_file(b_dot_basalt);
    const StructDefinition& A = a_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(rappresentation);
    navigator.visit_struct_definition(A);
}

TEST(Preprocessor, Recursive_Two_Struct_Dependency_With_Pointer_Accross_Different_Packages_With_Direct_Path) {
    
    FileRappresentation a_dot_basalt = {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage" }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "A", { }, {
                    StructDefinition::Field { "b",
                        PrimitiveType { Token { "B", "a.basalt", 1, 1, 1, Token::Type::type } } 
                    }
                }
            )
        },
        .func_defs = { }
    };

    auto a_type_with_package_prefix = BaseType { Token { "A", "b.basalt", 1, 1, 1, Token::Type::type }, {} };
    a_type_with_package_prefix.package_prefix = "apackage";

    
    FileRappresentation b_dot_basalt = {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "B", { }, {
                    StructDefinition::Field { "ptr",
                        PointerType { Token { "#", "b.basalt", 1, 1, 1, Token::Type::symbol },
                            a_type_with_package_prefix 
                        }
                    }
                }
            )
        },
        .func_defs = { }
    };

    ProgramRappresentation rappresentation;
    rappresentation.store_definitions_from_file(a_dot_basalt);
    rappresentation.store_definitions_from_file(b_dot_basalt);
    const StructDefinition& A = a_dot_basalt.type_defs[0].get<StructDefinition>();
    TypeDependencyNavigator navigator(rappresentation);
    navigator.visit_struct_definition(A);
}