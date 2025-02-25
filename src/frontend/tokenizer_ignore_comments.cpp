//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/tokenizer.hpp"
#include "syntax/specials.hpp"

void Tokenizer::update_multiline_comments_tracker() {
    if (char_pos + 1 < current_line.size()) {
        if (current_line[char_pos] == multiline_comment_opening[0] && current_line[char_pos+1] == multiline_comment_opening[1]) {
            char_pos = char_pos + 2;
            multiline_comments_tracker.push(make_token(multiline_comment_opening, Token::Type::multiline_comment));
        }
    }
}

void Tokenizer::ignore_multiline_comments() {
    while (!multiline_comments_tracker.empty() && char_pos < current_line.size()) {
        if (char_pos + 1 < current_line.size()) {
            std::string maybe_comment_sequence = current_line.substr(char_pos, 2);
            if (maybe_comment_sequence == multiline_comment_opening) {
                char_pos = char_pos + 2;
                Token comment_token = make_token(multiline_comment_opening, Token::Type::multiline_comment);
                multiline_comments_tracker.push(comment_token);
                continue;
            }
            else if (maybe_comment_sequence == multiline_comment_closing) {
                char_pos = char_pos + 2;
                multiline_comments_tracker.pop();
                continue;
            }
        }
        char_pos = char_pos + 1;
    }
}

void Tokenizer::handle_multiline_comments() {
    update_multiline_comments_tracker();
    ignore_multiline_comments();
}

void Tokenizer::handle_simple_comments() {
    if (char_pos < current_line.size() - 1) {
        if (current_line[char_pos] == single_line_comment[0] && current_line[char_pos+1] == single_line_comment[1]) {
            char_pos = current_line.size();
        }
    }
}