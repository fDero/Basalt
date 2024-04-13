
#include "errors/tokenization_errors.hpp"
#include "toolchain/tokenizer.hpp"
#include "language/syntax.hpp"

[[nodiscard]] std::optional<Token> Tokenizer::extract_number(){
    if (isdigit(current_line[char_pos])) {
        std::string buffer;
        bool floating = false;
        for (int i = char_pos; i < current_line.size(); i++){
            if (!isdigit(current_line[i]) && current_line[i] != '.') break;
            avoid_multiple_floating_points(floating, current_line[i], *this);
            floating = floating || (current_line[i] == '.');
            buffer.push_back(current_line[i]);
        }
        Token::Type type = (floating)? Token::Type::floating_literal : Token::Type::integer_literal;
        return make_token(buffer, type);
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<Token> Tokenizer::extract_text(){
    if (isalpha(current_line[char_pos])) {
        std::string buffer;
        for (int i = char_pos; i < current_line.size(); i++){
            if (!isalpha(current_line[i]) && !isdigit(current_line[i]) && current_line[i] != '_') break;
            buffer.push_back(current_line[i]);
        }
        Token::Type type = get_textual_token_type(buffer);
        return make_token(buffer, type);
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<Token> Tokenizer::extract_string(){
    if (string_opening_characters.find(current_line[char_pos]) != string_opening_characters.end()){
        std::string buffer;
        bool escape = false;
        buffer.push_back(current_line[char_pos]);
        for(int i = char_pos + 1; i < current_line.size() && !(current_line[char_pos] == current_line[i] && !escape); i++){
            escape = (current_line[i] == '\\' && !escape);
            buffer.push_back(current_line[i]);
        }
        ensure_string_gets_closed(buffer, *this);
        buffer.push_back(current_line[char_pos]);
        Token::Type type = (buffer[0] == '\'')? Token::Type::character_literal : Token::Type::string_literal;
        return make_token(buffer, type);
    }
    return std::nullopt;
}

[[nodiscard]] std::optional<Token> Tokenizer::extract_symbol(){
    int i = char_pos, j = char_pos + 1;
    Token::Type type = Token::Type::symbol;
    if(i >= current_line.size() || symbols.find(current_line[i]) == symbols.end()) return std::nullopt;
    if(j >= current_line.size() || symbols.find(current_line[j]) == symbols.end()) return make_token({current_line[i]}, type);
    if(combinations.find({current_line[i],current_line[j]}) == combinations.end()) return make_token({current_line[i]}, type);
    return make_token({current_line[i],current_line[j]}, type);
}

[[nodiscard]] std::optional<Token> Tokenizer::extract(){
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