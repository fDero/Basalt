
#pragma once
#include <gtest/gtest.h>
#include "../../include/toolchain/rappresentation.hpp"
#include "../../include/language/syntax.hpp"
#include "../../include/language/definitions.hpp"

inline StructDefinition make_struct_definition(const std::string& struct_name){
    StructDefinition test_struct {
        Token {
            struct_name, "test.basalt", 1, 2, 8,
            Token::Type::struct_keyword
        }
    };
    return test_struct;
}

inline FunctionDefinition make_function_definition(const std::string& func_name){
    return FunctionDefinition {
        Token {
            func_name, "test.basalt", 1, 2, 5,
            Token::Type::struct_keyword
        }
    };
}