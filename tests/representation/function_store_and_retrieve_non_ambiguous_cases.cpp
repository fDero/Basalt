
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

TEST(Representation, Simple_Function_Store_And_Retrieve) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", TypeSignatureFactory::Int}, { "y", TypeSignatureFactory::Int } },
                TypeSignatureFactory::Int
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

    FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
        FunctionCall {
            Token {
                "add", "test.basalt", 1, 1, 1,
                Token::Type::text
            },
            { int_literal_6, int_literal_6 },
            {}
        },
        { TypeSignatureFactory::Int, TypeSignatureFactory::Int },
        "test.basalt"
    });
}

TEST(Representation, Generic_Function_Store_And_Retrieve_No_Type_Inference) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", TypeSignatureFactory::Int}, { "y", TypeSignatureFactory::Int } },
                TypeSignatureFactory::Int
            ),

            FunctionDefinitionFactory::make_function_definition(
                "add", 
                { "T" },
                { { "x", TypeSignatureFactory::T }, { "y", TypeSignatureFactory::T } },
                TypeSignatureFactory::T
            )
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression float_literal_6 = IntLiteral {
        Token {
            "6.0", "test.basalt", 1, 1, 1,
            Token::Type::floating_literal
        }
    };

    FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
        FunctionCall {
            Token {
                "add", "test.basalt", 1, 1, 1,
                Token::Type::text
            },
            { float_literal_6, float_literal_6 },
            { TypeSignatureFactory::Float }
        },
        { TypeSignatureFactory::Float, TypeSignatureFactory::Float },
        "test.basalt"
    });
}

TEST(Representation, Generic_Function_Store_And_Retrieve_With_Type_Inference) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", TypeSignatureFactory::Int}, { "y", TypeSignatureFactory::Int } },
                TypeSignatureFactory::Int
            ),

            FunctionDefinitionFactory::make_function_definition(
                "add", 
                { "T" },
                { { "x", TypeSignatureFactory::T }, { "y", TypeSignatureFactory::T } },
                TypeSignatureFactory::Int
            )
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression float_literal_6 = IntLiteral {
        Token {
            "6.0", "test.basalt", 1, 1, 1,
            Token::Type::floating_literal
        }
    };

    FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
        FunctionCall {
            Token {
                "add", "test.basalt", 1, 1, 1,
                Token::Type::text
            },
            { float_literal_6, float_literal_6 },
            {} // no generics -> type inference
        },
        { TypeSignatureFactory::Float, TypeSignatureFactory::Float },
        "test.basalt"
    });
}

TEST(Representation, Generic_Function_Store_And_Retrieve_With_Bad_Concrete_Generics) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", TypeSignatureFactory::Int}, { "y", TypeSignatureFactory::Int } },
                TypeSignatureFactory::Int
            ),

            FunctionDefinitionFactory::make_function_definition(
                "add", 
                { "T" },
                { { "x", TypeSignatureFactory::T }, { "y", TypeSignatureFactory::T } },
                TypeSignatureFactory::T
            )
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression float_literal_6 = IntLiteral {
        Token {
            "6.0", "test.basalt", 1, 1, 1,
            Token::Type::floating_literal
        }
    };

    EXPECT_ANY_THROW({
        FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
            FunctionCall {
                Token {
                    "add", "test.basalt", 1, 1, 1,
                    Token::Type::text
                },
                { float_literal_6, float_literal_6 },
                { TypeSignatureFactory::String } 
            },
            { TypeSignatureFactory::Float, TypeSignatureFactory::Float },
            "test.basalt"
        });
    });
}

TEST(Representation, Generic_Function_Store_And_Retrieve_With_Bad_Arguments_And_Type_Inference) {
    
    FileRepresentation file {
        .file_metadata = { 
            .filename = "test.basalt",
            .packagename = "testpackage",
            .imports = {},
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics,
                { { "x", TypeSignatureFactory::Int}, { "y", TypeSignatureFactory::Int } },
                TypeSignatureFactory::Int
            ),

            FunctionDefinitionFactory::make_function_definition(
                "add", 
                { "T" },
                { { "x", TypeSignatureFactory::T }, { "y", TypeSignatureFactory::T } },
                TypeSignatureFactory::T
            )
        }
    };

    ProgramRepresentation program;
    program.store_definitions_from_file(file);

    Expression float_literal_6 = IntLiteral {
        Token {
            "6.0", "test.basalt", 1, 1, 1,
            Token::Type::floating_literal
        }
    };

    Expression string_literal_hello_world = StringLiteral {
        Token {
            "`Hello, world!`", "test.basalt", 1, 1, 1,
            Token::Type::string_literal
        }
    };

    EXPECT_ANY_THROW({
        FunctionDefinition add_function_definition = program.retrieve_func_definition(PrecompiledFunctionCall {
            FunctionCall {
                Token {
                    "add", "test.basalt", 1, 1, 1,
                    Token::Type::text
                },
                { float_literal_6, string_literal_hello_world },
                { } // no generics -> type inference 
            },
            { TypeSignatureFactory::Float, TypeSignatureFactory::String },
            "test.basalt"
        });
    });
}