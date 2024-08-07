
#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <optional>
#include <stack>
#include <regex>
#include "misc/debug_informations_aware_entity.h"

struct Token : public DebugInformationsAwareEntity {

    enum class Type {
        symbol, text, type, integer_literal, floating_literal, 
        string_literal, character_literal, boolean_literal, 
        return_keyword, break_keyword, continue_keyword, throw_keyword, try_keyword, 
        catch_keyword, attempt_keyword, multiline_comment, if_keyword, else_keyword,
        while_keyword, until_keyword, var_keyword, const_keyword, 
        func_keyword, struct_keyword, union_keyword, enum_keyword,
        package_keyword, import_keyword, alias_keyword,
        is_keyword, as_keyword
    };

    using DebugInformationsAwareEntity::line_number;
    using DebugInformationsAwareEntity::tok_number;
    using DebugInformationsAwareEntity::char_pos;
    
    std::string sourcetext;
    Type type;

    Token(
        const std::string& sourcetext,
        const std::string& filename,
        size_t in_line_number,
        size_t in_tok_number,
        size_t in_char_pos,
        Type type
    );
};

struct TokenizedFile {
    std::string filename;
    std::vector<Token> tokens;
};

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