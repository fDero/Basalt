
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

ProjectFileStructure single_file_project_with_list_and_number_defs2({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "List", "main.basalt", { "T" }, { StructDefinitionFactory::no_fields }
            ),
            UnionDefinitionFactory::make_union_definition(
                "Number", "main.basalt", { }, {
                    TypeSignatureFactory::Int,
                    TypeSignatureFactory::Float
                }
            )
        },
        .func_defs = { }
    }
});

TEST(Core, Inferred_Generics_Do_Not_Allow_Too_Much_Flexibility_On_Type_Parameters) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs2);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs2);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };

    TypeSignature list_of_T = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    TypeSignature list_of_IntOrFloat = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { IntOrFloat } };

    bool Int_is_compatible_with_T = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    bool List_Of_IntOrFloat_is_compatible_with_list_of_T = type_checker.validate_assignment(list_of_IntOrFloat, list_of_T);
    EXPECT_TRUE(Int_is_compatible_with_T);
    EXPECT_FALSE(List_Of_IntOrFloat_is_compatible_with_list_of_T);
}

TEST(Core, Inferred_Generics_Do_Not_Allow_Too_Much_Flexibility_On_Type_Parameters_Inverse) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs2);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs2);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };

    TypeSignature list_of_T = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    TypeSignature list_of_Int = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };

    bool IntOrFloat_is_compatible_with_T = type_checker.validate_assignment(IntOrFloat, TypeSignatureFactory::T);
    bool List_Of_Int_is_compatible_with_list_of_T = type_checker.validate_assignment(list_of_Int, list_of_T);
    EXPECT_TRUE(IntOrFloat_is_compatible_with_T);
    EXPECT_FALSE(List_Of_Int_is_compatible_with_list_of_T);
}

TEST(Core, Inferred_Generics_Works_Just_Fine_Nested_Type_Parameters) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs2);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs2);

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

TEST(Core, Inferred_Generics_Works_Just_Fine_Nested_Type_Parameters_Not_Quite_The_Same) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs2);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs2);

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