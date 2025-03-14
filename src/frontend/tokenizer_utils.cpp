//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "syntax/keywords.hpp"
#include "syntax/specials.hpp"
#include "errors/tokenization_errors.hpp"
#include "frontend/tokenizer.hpp"
#include "errors/internal_errors.hpp"

Token::Token (
    const std::string& sourcetext,
    const std::string& filename,
    size_t in_line_number,
    size_t in_tok_number,
    size_t in_char_pos,
    Type type
)
    :  DebugInformationsAwareEntity(
        filename, in_line_number,
        in_tok_number, in_char_pos
    )
    , sourcetext(sourcetext)
    , type(type) 
{}

Tokenizer::Tokenizer(const std::istringstream& inline_input)
    : token_input(std::make_unique<std::istringstream>(inline_input.str()))
    , filename("inline input token stream") {}

Tokenizer::Tokenizer(const std::string& file_input) 
    : token_input(std::make_unique<std::fstream>(file_input, std::ios::in))
    , filename(file_input) {}

void Tokenizer::inspect_for_unexpected_tokens() {
    std::string buffer;
    while (char_pos < current_line.size() && discardable.find(current_line[char_pos]) == discardable.end()) {
        buffer.push_back(current_line[char_pos++]);
    }
    if (!buffer.empty()) {
        char_pos = char_pos - 1;
        throw_unexpected_token(make_token(buffer, Token::Type::text));
    }
}

void Tokenizer::ignore_discardable_characters() {
    while (char_pos < current_line.size() && discardable.find(current_line[char_pos]) != discardable.end()) {
        char_pos = char_pos + 1;
    }
}

Token Tokenizer::make_token(const std::string& sourcetext, const Token::Type token_type) {
    return Token {
        sourcetext, filename, line_number, tok_number,
        static_cast<unsigned int>(char_pos - sourcetext.size() + 1),
        token_type
    };
}

DebugInformationsAwareEntity Tokenizer::make_coordinates() {
    return DebugInformationsAwareEntity { 
        filename,
        line_number, 
        tok_number,
        static_cast<unsigned int>(char_pos),
    };
}

Token::Type Tokenizer::get_textual_token_type(const std::string& sourcetext) {
    if (sourcetext == "true" || sourcetext == "false") return Token::Type::boolean_literal;
    if (isupper(sourcetext[0])) return Token::Type::type;
    auto keyword_search_outcome = keywords.find(sourcetext);
    auto not_found = keywords.end();
    return (keyword_search_outcome != not_found)?  
        keyword_search_outcome->second : Token::Type::text;
}

TokenizedFile Tokenizer::tokenize() {
    TokenizedFile tokenized_file;
    tokenized_file.filename = filename;
    std::vector<Token>& tokens = tokenized_file.tokens;
    while (std::getline(*token_input, current_line)) {
        line_number = line_number + 1;
        tok_number = char_pos = 0;
        while (char_pos < current_line.size()) {
            ignore_discardable_characters();
            std::optional<Token> token = extract();
            if (token.has_value()) {
                tokens.push_back(*token);
            }
            tok_number += (token.has_value() && token->type != Token::Type::multiline_comment);
            char_pos += ( (token.has_value())? token->sourcetext.size() : 0 );
        }
    }
    ensure_multiline_comments_get_closed(multiline_comments_tracker, make_coordinates());
    return tokenized_file;
}