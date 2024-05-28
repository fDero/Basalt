
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Int_Is_Compatible_With_Template_T) {
    ProgramRepresentation empty_program;
    AssignmentTypeChecker type_checker(empty_program);
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    EXPECT_TRUE(int_is_compatible_with_template_t);
}

TEST(Preprocessor, Int_And_Number_Are_Compatible_With_Template_T) {
    ProgramRepresentation simple_union_definition_program;
    simple_union_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
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
    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    bool number_is_compatible_with_template_t = type_checker.validate_assignment(number_type, TypeSignatureFactory::T);
    EXPECT_TRUE(int_is_compatible_with_template_t);
    EXPECT_TRUE(number_is_compatible_with_template_t);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_substituded, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Preprocessor, Number_And_Int_Are_Compatible_With_Template_T) {
    ProgramRepresentation simple_union_definition_program;
    simple_union_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
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
    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool number_is_compatible_with_template_t = type_checker.validate_assignment(number_type, TypeSignatureFactory::T);
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    EXPECT_TRUE(int_is_compatible_with_template_t);
    EXPECT_TRUE(number_is_compatible_with_template_t);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_substituded, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Preprocessor, String_And_Int_Cannot_Be_Compatible_With_T_Simultaneously) {
    ProgramRepresentation empty_program;
    AssignmentTypeChecker type_checker(empty_program);
    bool string_is_compatible_with_T = type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::T);
    bool int_is_compatible_with_T_after_string_was_assigned_to_it = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    EXPECT_TRUE(string_is_compatible_with_T);
    EXPECT_FALSE(int_is_compatible_with_T_after_string_was_assigned_to_it);
}

TEST(Preprocessor, List_Of_Ints_And_List_Of_Number_Are_Compatible_With_List_Of_T) {
    ProgramRepresentation simple_multi_definition_program;
    simple_multi_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T "}, { StructDefinitionFactory::no_fields }
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
    AssignmentTypeChecker type_checker(simple_multi_definition_program);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    TypeSignature list_of_Ts = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    bool list_of_numbers_compatible_with_list_of_Ts = type_checker.validate_assignment(list_of_numbers, list_of_Ts);
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(list_of_ints, list_of_Ts);
    EXPECT_TRUE(list_of_numbers_compatible_with_list_of_Ts);
    EXPECT_TRUE(int_is_compatible_with_template_t);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_substituded, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Preprocessor, List_Of_Ints_And_List_Of_Number_Are_Non_Mutually_Compatible_With_Each_Other) {
    ProgramRepresentation simple_union_definition_program;
    simple_union_definition_program.store_definitions_from_file(
        Filerepresentation {
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
    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    bool list_of_numbers_compatible_with_list_of_ints = type_checker.validate_assignment(list_of_numbers, list_of_ints);
    bool list_of_ints_compatible_with_list_of_numbers = type_checker.validate_assignment(list_of_ints, list_of_numbers);
    EXPECT_FALSE(list_of_numbers_compatible_with_list_of_ints);
    EXPECT_FALSE(list_of_ints_compatible_with_list_of_numbers);
}