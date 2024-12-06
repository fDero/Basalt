
#pragma once
#include "language/definitions.hpp"

struct FunctionDefinitionFactory {

    inline static const std::vector<std::string> no_generics = {};
    inline static const std::vector<FunctionDefinition::Argument > no_args = {}; 

    static FunctionDefinition make_void_function_definition(
        const std::string& function_name, 
        const std::string& filename,
        const std::vector<std::string> generics, 
        const std::vector<FunctionDefinition::Argument>& args
    ) {
        FunctionDefinition new_func_definition {
            Token {
                function_name, filename, 1, 2, 5,
                Token::Type::text
            },
        };
        new_func_definition.arguments = args;
        new_func_definition.template_generics_names = generics;
        return new_func_definition;
    }

    static FunctionDefinition make_function_definition(
        const std::string& function_name,
        const std::string& filename, 
        const std::vector<std::string> generics, 
        const std::vector<FunctionDefinition::Argument>& args,
        const std::optional<TypeSignature>& return_type
    ) {
        FunctionDefinition new_func_definition {
            Token {
                function_name, filename, 1, 2, 5,
                Token::Type::text
            },
        };
        new_func_definition.arguments = args;
        new_func_definition.template_generics_names = generics;
        new_func_definition.return_type = return_type;
        return new_func_definition;
    }

    static FunctionDefinition make_function_definition(
        const std::string& function_name,
        const std::string& filename, 
        const std::vector<std::string> generics, 
        const std::vector<FunctionDefinition::Argument>& args,
        const std::optional<TypeSignature>& return_type,
        const std::vector<Statement>& body
    ) {
        FunctionDefinition new_func_definition {
            Token {
                function_name, filename, 1, 2, 5,
                Token::Type::text
            },
        };
        new_func_definition.arguments = args;
        new_func_definition.template_generics_names = generics;
        new_func_definition.return_type = return_type;
        new_func_definition.code = body;
        return new_func_definition;
    }
};