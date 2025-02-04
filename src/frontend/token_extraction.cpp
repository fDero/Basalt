//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/tokenization_errors.hpp"
#include "frontend/tokenizer.hpp"
#include "syntax/symbols.hpp"

std::optional<Token> Tokenizer::extract_number() {
    if (isdigit(current_line[char_pos])) {
        std::string buffer;
        bool floating = false;
        for (size_t i = char_pos; i < current_line.size(); i++) {
            if (!isdigit(current_line[i]) && current_line[i] != '.') {
                break;
            }
            avoid_multiple_floating_points(floating, current_line[i], make_coordinates());
            floating = floating || (current_line[i] == '.');
            buffer.push_back(current_line[i]);
        }
        Token::Type type = (floating)
            ? Token::Type::floating_literal 
            : Token::Type::integer_literal;
        return make_token(buffer, type);
    }
    return std::nullopt;
}

std::optional<Token> Tokenizer::extract_text() {
    if (isalpha(current_line[char_pos])) {
        std::string buffer;
        for (size_t i = char_pos; i < current_line.size(); i++) {
            if (!isalpha(current_line[i]) && !isdigit(current_line[i]) && current_line[i] != '_') break;
            buffer.push_back(current_line[i]);
        }
        Token::Type type = get_textual_token_type(buffer);
        return make_token(buffer, type);
    }
    return std::nullopt;
}

std::optional<Token> Tokenizer::extract_string() {
    if (string_opening_characters.find(current_line[char_pos]) != string_opening_characters.end()) {
        std::string buffer;
        bool escape = false;
        buffer.push_back(current_line[char_pos]);
        for(size_t i = char_pos + 1; i < current_line.size(); i++) {
            buffer.push_back(current_line[i]);
            if (current_line[i] == buffer.front() && !escape) {
                break;
            }
            escape = (current_line[i] == '\\') && !escape;
        }
        ensure_string_gets_closed(buffer, make_coordinates());
        Token::Type type = (buffer[0] == '\'')
            ? Token::Type::character_literal 
            : Token::Type::string_literal;
        return make_token(buffer, type);
    }
    return std::nullopt;
}

std::optional<Token> Tokenizer::extract_symbol() {
    size_t i = char_pos, j = char_pos + 1;
    Token::Type type = Token::Type::symbol;
    if(i >= current_line.size() || symbols.find(current_line[i]) == symbols.end()) return std::nullopt;
    if(j >= current_line.size() || symbols.find(current_line[j]) == symbols.end()) return make_token({current_line[i]}, type);
    if(combinations.find({current_line[i],current_line[j]}) == combinations.end()) return make_token({current_line[i]}, type);
    return make_token({current_line[i],current_line[j]}, type);
}

std::optional<Token> Tokenizer::extract() {
    handle_multiline_comments();
    handle_simple_comments();
    ignore_discardable_characters();
    std::optional<Token> extracted; 
    if ((extracted = extract_text()).has_value())    return extracted;
    if ((extracted = extract_symbol()).has_value())  return extracted;
    if ((extracted = extract_number()).has_value())  return extracted;
    if ((extracted = extract_string()).has_value())  return extracted;
    inspect_for_unexpected_tokens();
    return std::nullopt;
}