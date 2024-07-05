
#include <gtest/gtest.h>
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
            FileRepresentation {
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
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_replaced, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Preprocessor, Number_And_Int_Are_Compatible_With_Template_T) {
    ProgramRepresentation simple_union_definition_program;
    simple_union_definition_program.store_definitions_from_file(
            FileRepresentation {
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
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_replaced, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Preprocessor, List_Of_Ints_And_List_Of_Number_Are_Compatible_With_List_Of_T) {
    ProgramRepresentation simple_multi_definition_program;
    simple_multi_definition_program.store_definitions_from_file(
            FileRepresentation {
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
    TypeSignature list_of_Us = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::U } };

    bool list_of_numbers_compatible_with_list_of_Ts = type_checker.validate_assignment(list_of_numbers, list_of_Ts);
    bool list_of_ints_compatible_with_list_of_Us = type_checker.validate_assignment(list_of_ints, list_of_Us);

    EXPECT_TRUE(list_of_numbers_compatible_with_list_of_Ts);
    EXPECT_TRUE(list_of_ints_compatible_with_list_of_Us);
    ASSERT_EQ(type_checker.get_generic_substitution_rules().size(), 2);
}

TEST(Preprocessor, List_Of_Ints_And_List_Of_Number_Are_Non_Mutually_Compatible_With_Each_Other) {
    ProgramRepresentation simple_multi_definition_program;
    simple_multi_definition_program.store_definitions_from_file(
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

    AssignmentTypeChecker type_checker(simple_multi_definition_program);
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    bool list_of_numbers_compatible_with_list_of_ints = type_checker.validate_assignment(list_of_numbers, list_of_ints);
    bool list_of_ints_compatible_with_list_of_numbers = type_checker.validate_assignment(list_of_ints, list_of_numbers);
    EXPECT_FALSE(list_of_numbers_compatible_with_list_of_ints);
    EXPECT_FALSE(list_of_ints_compatible_with_list_of_numbers);
}


TEST(Preprocessor, List_Of_List_Of_Ints_Is_Compatible_With_List_Of_List_Of_T) {
    ProgramRepresentation simple_struct_definition_program;
    simple_struct_definition_program.store_definitions_from_file(
            FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_struct_definition_program);
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

TEST(Preprocessor, List_Of_Ints_is_not_compatible_with_List_Of_Ints_When_Symbols_Are_From_Different_Packages) {
    ProgramRepresentation simple_struct_definition_program;
    simple_struct_definition_program.store_definitions_from_file(
            FileRepresentation {
            .file_metadata = {
                .filename = "a.basalt",
                .packagename = "apackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                )
            },
            .func_defs = { }
        }
    );
    simple_struct_definition_program.store_definitions_from_file(
            FileRepresentation {
            .file_metadata = {
                .filename = "b.basalt",
                .packagename = "bpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_struct_definition_program);
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