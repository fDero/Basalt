
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(Preprocessor, Int_Is_Mutually_Compatible_With_AliasedInt) {
    ProgramRepresentation simple_alias_definition_program;
    simple_alias_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                TypeAliasFactory::make_type_alias(
                    "AliasedInt", { }, TypeSignatureFactory::Int
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_alias_definition_program);
    TypeSignature aliased_int = CustomType { Token { "AliasedInt", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool aliased_int_is_compatible_with_int = type_checker.validate_assignment(aliased_int, TypeSignatureFactory::Int);
    bool int_is_compatible_with_aliased_int = type_checker.validate_assignment(TypeSignatureFactory::Int, aliased_int);
    EXPECT_TRUE(aliased_int_is_compatible_with_int);
    EXPECT_TRUE(int_is_compatible_with_aliased_int);
}

TEST(Preprocessor, Int_Is_Mutually_Compatible_With_Generic_Alias_Instantiated_With_Int) {
    ProgramRepresentation simple_alias_definition_program;
    simple_alias_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                TypeAliasFactory::make_type_alias(
                    "SameAs", { "T" }, TypeSignatureFactory::T
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_alias_definition_program);
    TypeSignature aliased_int = CustomType { Token { "SameAs", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };
    bool aliased_int_is_compatible_with_int = type_checker.validate_assignment(aliased_int, TypeSignatureFactory::Int);
    bool int_is_compatible_with_aliased_int = type_checker.validate_assignment(TypeSignatureFactory::Int, aliased_int);
    EXPECT_TRUE(aliased_int_is_compatible_with_int);
    EXPECT_TRUE(int_is_compatible_with_aliased_int);
}

TEST(Preprocessor, Pair_Of_Int_Int_Is_Compatible_With_Pair_Of_One_Generic_T_Via_Alias) {
    ProgramRepresentation simple_alias_definition_program;
    simple_alias_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = {
                StructDefinitionFactory::make_struct_definition(
                    "Pair", { "T", "U" }, {
                        StructDefinition::Field { "first", TypeSignatureFactory::T },
                        StructDefinition::Field { "second", TypeSignatureFactory::U }
                    }
                ),
                TypeAliasFactory::make_type_alias(
                    "Pair", { "T" }, TypeSignature {
                        CustomType { Token { "Pair", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                            TypeSignatureFactory::T, 
                            TypeSignatureFactory::T 
                        } }
                    }
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_alias_definition_program);
    TypeSignature pair_int_int = CustomType { 
        Token { "Pair", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::Int, 
            TypeSignatureFactory::Int 
        } 
    };
    TypeSignature pair_of_Ts = CustomType { 
        Token { "Pair", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::T 
        } 
    };
    bool pair_int_int_compatible_with_pair_of_Ts = type_checker.validate_assignment(pair_int_int, pair_of_Ts);
    EXPECT_TRUE(pair_int_int_compatible_with_pair_of_Ts);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_substituded, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<PrimitiveType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<PrimitiveType>().type_name, "Int");
}

TEST(Preprocessor, Pair_Of_Int_Is_Compatible_With_Pair_Of_T_U) {
    ProgramRepresentation simple_alias_definition_program;
    simple_alias_definition_program.store_definitions_from_file(
        Filerepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = {
                StructDefinitionFactory::make_struct_definition(
                    "Pair", { "T", "U" }, {
                        StructDefinition::Field { "first", TypeSignatureFactory::T },
                        StructDefinition::Field { "second", TypeSignatureFactory::U }
                    }
                ),
                TypeAliasFactory::make_type_alias(
                    "Pair", { "T" }, TypeSignature {
                        CustomType { Token { "Pair", "main.basalt", 1, 1, 1, Token::Type::type }, { 
                            TypeSignatureFactory::T, 
                            TypeSignatureFactory::T 
                        } }
                    }
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_alias_definition_program);
    TypeSignature pair_T_U = CustomType { 
        Token { "Pair", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::T, 
            TypeSignatureFactory::U 
        } 
    };
    TypeSignature pair_of_ints = CustomType { 
        Token { "Pair", "main.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::Int
        } 
    };
    bool pair_int_compatible_with_pair_of_T_U = type_checker.validate_assignment(pair_of_ints, pair_T_U);
    EXPECT_TRUE(pair_int_compatible_with_pair_of_T_U);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().size(), 2);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().front().to_be_substituded, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().front().replacement.is<PrimitiveType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().front().replacement.get<PrimitiveType>().type_name, "Int");
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_substituded, "U");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<PrimitiveType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<PrimitiveType>().type_name, "Int");
}

TEST(Preprocessor, List_Of_Aliased_Ints_And_List_Of_Number_Are_Compatible_With_List_Of_T) {
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
                TypeAliasFactory::make_type_alias(
                    "AliasedInt", { }, TypeSignatureFactory::Int
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
    TypeSignature aliased_int = CustomType { Token { "AliasedInt", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_aliased_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { aliased_int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    TypeSignature list_of_Ts = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    bool list_of_numbers_compatible_with_list_of_Ts = type_checker.validate_assignment(list_of_numbers, list_of_Ts);
    bool list_of_aliased_ints_compatible_with_list_of_Ts = type_checker.validate_assignment(list_of_aliased_ints, list_of_Ts);
    EXPECT_TRUE(list_of_numbers_compatible_with_list_of_Ts);
    EXPECT_TRUE(list_of_aliased_ints_compatible_with_list_of_Ts);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().to_be_substituded, "T");
    ASSERT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<CustomType>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<CustomType>().type_name, "Number");
}

TEST(Preprocessor, List_Of_Aliased_Ints_And_List_Of_Number_Are_Non_Mutually_Compatible_With_Each_Other) {
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
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                ),
                TypeAliasFactory::make_type_alias(
                    "AliasedInt", { }, TypeSignatureFactory::Int
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
    TypeSignature aliased_int = CustomType { Token { "AliasedInt", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature number_type = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature list_of_aliased_ints = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { aliased_int } };
    TypeSignature list_of_numbers = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { number_type } };
    bool list_of_numbers_compatible_with_list_of_aliased_ints = type_checker.validate_assignment(list_of_numbers, list_of_aliased_ints);
    bool list_of_aliased_ints_compatible_with_list_of_numbers = type_checker.validate_assignment(list_of_aliased_ints, list_of_numbers);
    EXPECT_FALSE(list_of_numbers_compatible_with_list_of_aliased_ints);
    EXPECT_FALSE(list_of_aliased_ints_compatible_with_list_of_numbers);
}