
#include <gtest/gtest.h>
#include "toolchain/representation.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/function_definition_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

ProjectFileStructure multi_file_project_with_multiple_function_definitions ({
    FileRepresentation {
        .file_metadata = { 
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage", "cpackage" },
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Int },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Int }
                },
                TypeSignatureFactory::Int
            ),
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Float },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Float }
                },
                TypeSignatureFactory::Float
            )
        }
    },
    FileRepresentation {
        .file_metadata = { 
            .filename = "b.basalt",
            .packagename = "bpackage",
            .imports = { },
        },
        .type_defs = {
            
        },
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", { "T" }, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::T },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::T }
                },
                TypeSignatureFactory::T
            )
        }
    },
    FileRepresentation {
        .file_metadata = { 
            .filename = "c.basalt",
            .packagename = "cpackage",
            .imports = { },
        },
        .type_defs = {
            
        },
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::IntOrFloat},
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::IntOrFloat }
                },
                TypeSignatureFactory::IntOrFloat
            )
        }
    }
});

TEST(Representation, Retrieve_Function_Overload_Sets_Ids_For_The_Add_Function_From_A_Dot_Basalt_Without_Explicit_Generics) {
    
    FunctionOverloadsRegister function_register(multi_file_project_with_multiple_function_definitions);
    
    FunctionCall function_call {
        Token { "add", "a.basalt", 1, 1, 1, Token::Type::text },
        { 
            IntLiteral { Token { "6", "a.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "7", "a.basalt", 1, 1, 1, Token::Type::integer_literal } } 
        }, 
        {
            //no explicit type parameters section
        }
    };

    std::vector<std::string> overload_sets_ids = function_register.retrieve_overload_sets_ids(function_call);
    
    ASSERT_EQ(overload_sets_ids.size(), 3);
}

TEST(Representation, Retrieve_Function_Overload_Sets_Ids_For_The_Add_Function_From_B_Dot_Basalt_Without_Explicit_Generics) {
    
    FunctionOverloadsRegister function_register(multi_file_project_with_multiple_function_definitions);
    
    FunctionCall function_call {
        Token { "add", "b.basalt", 1, 1, 1, Token::Type::text },
        { 
            IntLiteral { Token { "6", "b.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "7", "b.basalt", 1, 1, 1, Token::Type::integer_literal } } 
        }, 
        {
            //no explicit type parameters section
        }
    };

    std::vector<std::string> overload_sets_ids = function_register.retrieve_overload_sets_ids(function_call);

    ASSERT_EQ(overload_sets_ids.size(), 1);
}

TEST(Representation, Retrieve_Function_Overload_Sets_Ids_For_The_Add_Function_From_C_Dot_Basalt_Without_Explicit_Generics) {
    
    FunctionOverloadsRegister function_register(multi_file_project_with_multiple_function_definitions);
    
    FunctionCall function_call {
        Token { "add", "c.basalt", 1, 1, 1, Token::Type::text },
        { 
            IntLiteral { Token { "6", "c.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "7", "c.basalt", 1, 1, 1, Token::Type::integer_literal } } 
        }, 
        {
            //no explicit type parameters section
        }
    };

    std::vector<std::string> overload_sets_ids = function_register.retrieve_overload_sets_ids(function_call);
    
    ASSERT_EQ(overload_sets_ids.size(), 1);
}

TEST(Representation, Retrieve_Function_Overload_Sets_Ids_For_The_Add_Function_From_A_Dot_Basalt_With_Explicit_Generics) {
    
    FunctionOverloadsRegister function_register(multi_file_project_with_multiple_function_definitions);
    
    FunctionCall function_call {
        Token { "add", "a.basalt", 1, 1, 1, Token::Type::text },
        { 
            IntLiteral { Token { "6", "a.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "7", "a.basalt", 1, 1, 1, Token::Type::integer_literal } } 
        }, 
        {
            TypeSignatureFactory::Int
        }
    };

    std::vector<std::string> overload_sets_ids = function_register.retrieve_overload_sets_ids(function_call);
    
    ASSERT_EQ(overload_sets_ids.size(), 1);
}

TEST(Representation, Retrieve_Function_Overload_Sets_Ids_For_The_Add_Function_From_B_Dot_Basalt_With_Explicit_Generics) {
    
    FunctionOverloadsRegister function_register(multi_file_project_with_multiple_function_definitions);
    
    FunctionCall function_call {
        Token { "add", "b.basalt", 1, 1, 1, Token::Type::text },
        { 
            IntLiteral { Token { "6", "b.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "7", "b.basalt", 1, 1, 1, Token::Type::integer_literal } } 
        }, 
        {
            TypeSignatureFactory::Int
        }
    };

    std::vector<std::string> overload_sets_ids = function_register.retrieve_overload_sets_ids(function_call);

    ASSERT_EQ(overload_sets_ids.size(), 1);
}

TEST(Representation, Retrieve_Function_Overload_Sets_Ids_For_The_Add_Function_From_C_Dot_Basalt_With_Explicit_Generics) {
    
    FunctionOverloadsRegister function_register(multi_file_project_with_multiple_function_definitions);
    
    FunctionCall function_call {
        Token { "add", "c.basalt", 1, 1, 1, Token::Type::text },
        { 
            IntLiteral { Token { "6", "c.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "7", "c.basalt", 1, 1, 1, Token::Type::integer_literal } } 
        }, 
        {
            TypeSignatureFactory::Int
        }
    };

    std::vector<std::string> overload_sets_ids = function_register.retrieve_overload_sets_ids(function_call);
    
    ASSERT_EQ(overload_sets_ids.size(), 0);
}