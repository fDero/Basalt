
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure single_file_project_with_list_and_number_defs({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "List", "main.basalt", { "T "}, { StructDefinitionFactory::no_fields }
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

ProjectFileStructure multi_file_project_with_ambiguous_list_defs({
    FileRepresentation {
        .file_metadata = {
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "List", "a.basalt", { "T" }, { StructDefinitionFactory::no_fields }
            )
        },
        .func_defs = { }
    },
    FileRepresentation {
        .file_metadata = {
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { }
        },
        .type_defs = { 
            StructDefinitionFactory::make_struct_definition(
                "List", "b.basalt", { "T" }, { StructDefinitionFactory::no_fields }
            )
        },
        .func_defs = { }
    }
});

TEST(Core, Int_Is_Compatible_With_Template_T) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister empty_type_register(empty_project);
    AssignmentTypeChecker type_checker(empty_type_register, empty_project);
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    EXPECT_TRUE(int_is_compatible_with_template_t);
}

TEST(Core, Int_And_Number_Are_Compatible_With_Template_T) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    bool number_is_compatible_with_template_t = type_checker.validate_assignment(number_type, TypeSignatureFactory::T);
    EXPECT_TRUE(int_is_compatible_with_template_t);
    EXPECT_TRUE(number_is_compatible_with_template_t);
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().to_be_replaced, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules()->back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Core, Number_And_Int_Are_Compatible_With_Template_T) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool number_is_compatible_with_template_t = type_checker.validate_assignment(number_type, TypeSignatureFactory::T);
    bool int_is_compatible_with_template_t = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    EXPECT_TRUE(int_is_compatible_with_template_t);
    EXPECT_TRUE(number_is_compatible_with_template_t);
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().to_be_replaced, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules()->back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Core, List_Of_Ints_And_List_Of_Number_Are_Compatible_With_List_Of_T) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    TypeSignature list_of_Ts = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    TypeSignature list_of_Us = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::U } };

    bool list_of_numbers_compatible_with_list_of_Ts = type_checker.validate_assignment(list_of_numbers, list_of_Ts);
    bool list_of_ints_compatible_with_list_of_Us = type_checker.validate_assignment(list_of_ints, list_of_Us);

    EXPECT_TRUE(list_of_numbers_compatible_with_list_of_Ts);
    EXPECT_TRUE(list_of_ints_compatible_with_list_of_Us);
    ASSERT_EQ(type_checker.get_generic_substitution_rules()->size(), 2);
}

TEST(Core, List_Of_Ints_And_List_Of_Number_Are_Non_Mutually_Compatible_With_Each_Other) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    bool list_of_numbers_compatible_with_list_of_ints = type_checker.validate_assignment(list_of_numbers, list_of_ints);
    bool list_of_ints_compatible_with_list_of_numbers = type_checker.validate_assignment(list_of_ints, list_of_numbers);
    EXPECT_FALSE(list_of_numbers_compatible_with_list_of_ints);
    EXPECT_FALSE(list_of_ints_compatible_with_list_of_numbers);
}


TEST(Core, List_Of_List_Of_Ints_Is_Compatible_With_List_Of_List_Of_T) {
    TypeDefinitionsRegister type_register(single_file_project_with_list_and_number_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_list_and_number_defs);
    TypeSignature list_of_list_of_Ts = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, {
        CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } }
    } };
    TypeSignature list_of_list_of_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, {
        CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } }
    } };
    bool list_of_list_of_ints_compatible_with_list_of_list_of_Ts = 
        type_checker.validate_assignment(list_of_list_of_ints, list_of_list_of_Ts);
    EXPECT_TRUE(list_of_list_of_ints_compatible_with_list_of_list_of_Ts);
}

TEST(Core, List_Of_Ints_is_not_compatible_with_List_Of_Ints_When_Symbols_Are_From_Different_Packages) {
    TypeDefinitionsRegister type_register(multi_file_project_with_ambiguous_list_defs);
    AssignmentTypeChecker type_checker(type_register, multi_file_project_with_ambiguous_list_defs);
    CustomType list_of_list_of_ints1 = CustomType { Token { "List", "a.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int
    } };
    CustomType list_of_list_of_ints2 = CustomType { Token { "List", "a.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int
    } };
    list_of_list_of_ints2.package_prefix = "bpackage";
    bool list_of_ints_are_not_compatible_when_symbols_are_from_different_packages = 
        type_checker.validate_assignment(list_of_list_of_ints1, list_of_list_of_ints2);
    EXPECT_FALSE(list_of_ints_are_not_compatible_when_symbols_are_from_different_packages);
}