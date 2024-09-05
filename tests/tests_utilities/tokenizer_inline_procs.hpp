
#pragma once
#include <gtest/gtest.h>
#include "frontend/tokenizer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

inline std::pair<std::vector<std::string>, std::string> 
    make_testable_input(const std::vector<std::string>& tokens_text) {
        std::string inline_input;
        for (const std::string& token_text : tokens_text) {
            inline_input += " " + token_text;
        } 
        return { tokens_text, inline_input };   
}
