
#include "frontend/tokenizer.hpp"
#include "language/syntax.hpp"

void Tokenizer::update_multiline_comments_tracker() {
    if (char_pos + 1 < current_line.size()) {
        if (current_line[char_pos] == multiline_comment_opening[0] && current_line[char_pos+1] == multiline_comment_opening[1]) {
            char_pos = char_pos + 2;
            multiline_comments_tracker.push(make_token(multiline_comment_opening, Token::Type::multiline_comment));
        }
    }
}

void Tokenizer::ignore_multiline_comments() {
    while (!multiline_comments_tracker.empty() && char_pos + 1 < current_line.size()) {
        if (current_line[char_pos] == multiline_comment_opening[0] && current_line[char_pos+1] == multiline_comment_opening[1]) {
            char_pos = char_pos + 2;
            multiline_comments_tracker.push(make_token(multiline_comment_opening, Token::Type::multiline_comment));
        }
        else if (current_line[char_pos] == multiline_comment_closing[0] && current_line[char_pos+1] == multiline_comment_closing[1]) {
            char_pos = char_pos + 2;
            multiline_comments_tracker.pop();
        }
        else {
            char_pos = char_pos + 1;
        }
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