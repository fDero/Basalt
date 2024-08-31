/**
 * @file error-types.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the custom exceptions used internally by this codebase.
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "toolchain/tokenizer.hpp"
#include <string>
#include <exception>

struct CommandLineError : public std::exception {
    
    std::string error_message;
    
    CommandLineError(const std::string& message) 
        : error_message(message) { }
    
    [[nodiscard]] const char* what() const noexcept override {
        return error_message.c_str();
    }
};

struct TokenizationError : public std::exception {
    std::string error_message;
    std::string sourcetext;
    std::string filename;
    size_t line_number;
    size_t tok_number;
    size_t char_pos;

    TokenizationError(
        const std::string& message,
        const std::string& source,
        const std::string& file,
        size_t in_line_number,
        size_t in_token_number,
        size_t in_char_pos
    )
        : error_message(message), sourcetext(source)
        , filename(file), line_number(in_line_number)
        , tok_number(in_token_number), char_pos(in_char_pos)
    {}

    [[nodiscard]] const char* what() const noexcept override {
        return error_message.c_str();
    }
};

struct ParsingError : public std::exception {
    std::string error_message;
    std::string sourcetext;
    std::string filename;
    size_t line_number;
    size_t tok_number;
    size_t char_pos;

    ParsingError(
        const std::string& message,
        const std::string& source,
        const std::string& file,
        size_t in_line_number,
        size_t in_token_number,
        size_t in_char_pos
    )
        : error_message(message), sourcetext(source)
        , filename(file), line_number(in_line_number)
        , tok_number(in_token_number), char_pos(in_char_pos)
    {}

    ParsingError(const std::string& message, const Token& token)
        : error_message(message), sourcetext(token.sourcetext)
        , filename(token.filename), line_number(token.line_number)
        , tok_number(token.tok_number), char_pos(token.char_pos) 
    {}

    ParsingError(const std::string& message, const std::optional<Token>& token_opt)
        : error_message(message) 
    {
        if (token_opt.has_value()) {
            const Token& token = token_opt.value();
            sourcetext = token.sourcetext;
            filename = token.filename; 
            line_number = token.line_number;
            tok_number = token.tok_number;
            char_pos = token.char_pos;
        } 
    }
    
    [[nodiscard]] const char* what() const noexcept override {
        return error_message.c_str();
    }
};

struct InternalError : public std::exception {
    
    std::string error_message;

    InternalError(const std::string& message)
        : error_message(message) {}

    [[nodiscard]] const char* what() const noexcept override {
        return error_message.c_str();
    }
};