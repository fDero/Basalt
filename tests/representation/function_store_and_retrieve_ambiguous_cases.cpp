
#include <gtest/gtest.h>
#include "toolchain/representation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/function_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"
#include "../tests_utilities/type_queries.hpp"
#include "../tests_utilities/union_definition_factory.hpp"

TEST(Representation, Retrieve_Function_Definition_From_Apparently_Ambiguous_Case) {
    
    TypeSignature number_type = CustomType { Token { "Number", "test.basalt", 1, 1, 1, Token::Type::type }, {} };

    Filerepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            UnionDefinitionFactory::make_union_definition(
                "Number", { }, {
                    TypeSignatureFactory::Int,
                    TypeSignatureFactory::Float
                }
            )
        },
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", TypeSignatureFactory::Int}, { "y", TypeSignatureFactory::Int } },
                TypeSignatureFactory::Int
            ),
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", number_type}, { "y", number_type } },
                number_type
            )
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression int_literal_6 = IntLiteral {
        Token {
            "6", "test.basalt", 1, 1, 1,
            Token::Type::integer_literal
        }
    };

    Expression float_literal_pi = IntLiteral {
        Token {
            "3.14", "test.basalt", 1, 1, 1,
            Token::Type::floating_literal
        }
    };

    FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
        FunctionCall {
            Token {
                "add", "test.basalt", 1, 1, 1,
                Token::Type::text
            },
            { int_literal_6, float_literal_pi },
            {}
        },
        { TypeSignatureFactory::Int, TypeSignatureFactory::Float },
        "test.basalt"
    });
}

TEST(Representation, Retrieve_Function_Definition_From_Apparently_Ambiguous_Case_With_More_Specific_Generic_Function) {
    
    TypeSignature list_of_list_of_T = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
                TypeSignatureFactory::T
            } }
        } }
    ;

    TypeSignature list_of_list_of_int = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
                TypeSignatureFactory::Int
            } }
        } }
    ;

    TypeSignature list_of_T = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::T
        } }
    ;

    Filerepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "List", { "T" }, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "f", 
                { "T" },
                { { "x", list_of_list_of_T } },
                std::nullopt
            ),
            FunctionDefinitionFactory::make_function_definition(
                "f", 
                { "T" },
                { { "x", list_of_T } },
                std::nullopt
            ),
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression list_expr = Identifier {
        Token {
            "my_example_list", "test.basalt", 1, 1, 1,
            Token::Type::text
        }
    };

    FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
        FunctionCall {
            Token {
                "f", "test.basalt", 1, 1, 1,
                Token::Type::text
            },
            { list_expr },
            {}
        },
        { list_of_list_of_int },
        "test.basalt"
    });
}

TEST(Representation, Retrieve_Function_Definition_From_Truly_Ambiguous_Case_Causes_Error) {
    
    TypeSignature list_of_list_of_T = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
                TypeSignatureFactory::T
            } }
        } }
    ;

    TypeSignature list_of_list_of_int = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
                TypeSignatureFactory::Int
            } }
        } }
    ;

    TypeSignature list_of_T = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::T
        } }
    ;

    TypeSignature list_of_U = 
        CustomType { Token { "List", "test.basalt", 1, 1, 1, Token::Type::type }, { 
            TypeSignatureFactory::U
        } }
    ;

    Filerepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {
            StructDefinitionFactory::make_struct_definition(
                "List", { "T" }, 
                StructDefinitionFactory::no_fields
            )
        },
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "f", 
                { "T" },
                { { "x", list_of_list_of_T }, { "y", TypeSignatureFactory::U } },
                std::nullopt
            ),
            FunctionDefinitionFactory::make_function_definition(
                "f", 
                { "T" },
                { { "x", list_of_T }, { "y", list_of_U } },
                std::nullopt
            ),
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression list_expr = Identifier {
        Token {
            "my_example_list", "test.basalt", 1, 1, 1,
            Token::Type::text
        }
    };

    EXPECT_ANY_THROW({
        FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
            FunctionCall {
                Token {
                    "f", "test.basalt", 1, 1, 1,
                    Token::Type::text
                },
                { list_expr },
                {}
            },
            { list_of_list_of_int, list_of_list_of_int },
            "test.basalt"
        });
    });
}