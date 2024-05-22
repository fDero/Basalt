
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Number_Union_Is_Compatible_With_Itself) {
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
    TypeSignature number_type = BaseType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool number_is_compatible_with_itself = type_checker.validate_assignment(number_type, number_type);
    EXPECT_TRUE(number_is_compatible_with_itself);
}

TEST(Preprocessor, Int_Compatible_With_Number_Union) {
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
    TypeSignature number_type = BaseType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool int_compatible_with_number = type_checker.validate_assignment(TypeSignatureFactory::Int, number_type);
    bool number_compatible_with_int = type_checker.validate_assignment(number_type, TypeSignatureFactory::Int);
    EXPECT_TRUE(int_compatible_with_number);
    EXPECT_FALSE(number_compatible_with_int);
}

TEST(Preprocessor, Number_Union_Compatible_With_Primitive_Union) {
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
                ),
                UnionDefinitionFactory::make_union_definition(
                    "Primitive", { }, {
                        TypeSignatureFactory::Int,
                        TypeSignatureFactory::Float,
                        TypeSignatureFactory::Bool,
                        TypeSignatureFactory::String,
                        TypeSignatureFactory::RawString,
                        TypeSignatureFactory::Char,
                        TypeSignatureFactory::Byte
                    }
                )
            },
            .func_defs = { }
        }
    );

    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature number_type = BaseType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    TypeSignature primitive_type = BaseType { Token { "Primitive", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool number_compatible_with_primitive = type_checker.validate_assignment(number_type, primitive_type);
    bool primitive_compatible_with_number = type_checker.validate_assignment(primitive_type, number_type);
    EXPECT_TRUE(number_compatible_with_primitive);
    EXPECT_FALSE(primitive_compatible_with_number);
}

TEST(Preprocessor, Int_Compatible_With_Primitive_When_Int_Implicitly_Named_Via_Number) {
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
                ),
                UnionDefinitionFactory::make_union_definition(
                    "Primitive", { }, {
                        BaseType { 
                            Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, {} 
                        },
                        TypeSignatureFactory::Bool,
                        TypeSignatureFactory::String,
                        TypeSignatureFactory::RawString,
                        TypeSignatureFactory::Char,
                        TypeSignatureFactory::Byte
                    }
                )
            },
            .func_defs = { }
        }
    );

    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature primitive_type = BaseType { Token { "Primitive", "main.basalt", 1, 1, 1, Token::Type::type }, {} };
    bool int_compatible_with_primitive = type_checker.validate_assignment(TypeSignatureFactory::Int, primitive_type);
    EXPECT_TRUE(int_compatible_with_primitive);
}

TEST(Preprocessor, Int_Compatible_With_Generic_Either_Union) {
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
                    "Either", { "T", "U" }, {
                        TypeSignatureFactory::T,
                        TypeSignatureFactory::U
                    }
                )
            },
            .func_defs = { }
        }
    );

    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature either_int_or_float = BaseType { 
        Token { "Either", "main.basalt", 1, 1, 1, Token::Type::type }, {
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        } 
    };
    bool int_compatible_with_either_int_or_float = type_checker.validate_assignment(TypeSignatureFactory::Int, either_int_or_float);
    EXPECT_TRUE(int_compatible_with_either_int_or_float);
}