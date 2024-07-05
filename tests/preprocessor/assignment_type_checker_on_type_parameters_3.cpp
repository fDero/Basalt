
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(Preprocessor, Inferred_Generics_Works_Just_Fine_Nested_Type_Parameters) {
    ProgramRepresentation empty_program;
    empty_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                ),
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(empty_program);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };

    TypeSignature list_of_list_of_list_of_T = 
        CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                    TypeSignatureFactory::T
                } }
            } }
        }    
    };

    TypeSignature list_of_list_of_list_of_Int = 
        CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                    TypeSignatureFactory::Int
                } }
            } }
        }    
    };

    EXPECT_TRUE(type_checker.validate_assignment(list_of_list_of_list_of_Int, list_of_list_of_list_of_T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T));
}

TEST(Preprocessor, Inferred_Generics_Works_Just_Fine_Nested_Type_Parameters_Not_Quite_The_Same) {
    ProgramRepresentation empty_program;
    empty_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                ),
                UnionDefinitionFactory::make_union_definition(
                    "Number", { }, {
                        TypeSignatureFactory::Int,
                        TypeSignatureFactory::Float
                    }
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(empty_program);

    TypeSignature number = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, { } };

    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };

    TypeSignature list_of_list_of_list_of_IntOrFloat = 
        CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                    IntOrFloat
                } }
            } }
        }    
    };

    TypeSignature list_of_list_of_list_of_Number = 
        CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                   number
                } }
            } }
        }    
    };

    ASSERT_TRUE(type_checker.validate_assignment(number, IntOrFloat));
    ASSERT_TRUE(type_checker.validate_assignment(IntOrFloat, number));
    EXPECT_TRUE(type_checker.validate_assignment(list_of_list_of_list_of_Number, list_of_list_of_list_of_IntOrFloat));
}