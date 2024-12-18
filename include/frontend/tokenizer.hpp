//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>

#include "frontend/token.hpp"
#include "frontend/tokenized_file.hpp"

class Tokenizer {

    private: 
        std::unique_ptr<std::istream> token_input;
        std::string filename;
        std::string current_line;
        unsigned long line_number = 0;
        unsigned int tok_number = 0;
        unsigned int char_pos = 0;

        std::stack<Token> multiline_comments_tracker;
        void update_multiline_comments_tracker();
        void ignore_multiline_comments();
        void handle_multiline_comments();
        void handle_simple_comments(); 

        void ignore_discardable_characters();
        void inspect_for_unexpected_tokens();
        [[nodiscard]] Token make_token(const std::string& sourcetext, const Token::Type type);
        [[nodiscard]] Token::Type get_textual_token_type(const std::string& sourcetext);

    public:
        Tokenizer(const std::istringstream& inline_input);
        Tokenizer(const std::string& file_input);

        [[nodiscard]] TokenizedFile tokenize();
        [[nodiscard]] std::string get_current_line() const;
        [[nodiscard]] std::string get_filename() const;
        [[nodiscard]] size_t get_line_number() const;
        [[nodiscard]] size_t get_tok_number() const;
        [[nodiscard]] size_t get_char_pos() const;

        [[nodiscard]] std::optional<Token> extract_number();
        [[nodiscard]] std::optional<Token> extract_text();
        [[nodiscard]] std::optional<Token> extract_string();
        [[nodiscard]] std::optional<Token> extract_symbol();
        [[nodiscard]] std::optional<Token> extract();
};