
#pragma once
#include "toolchain/tokenizer.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

inline const std::string single_line_comment = "//";
inline const std::string multiline_comment_opening = "/*";
inline const std::string multiline_comment_closing = "*/";
inline const std::string namespace_concatenation = "::";
inline const std::string address_operator = "&";
inline const std::string pointer_dereference_operator = "#";
inline const std::string pointer_type_symbol = "#";
inline const std::string slice_type_symbol = "$";
inline const std::string square_brackets_access = "[square-brackets-access]";
inline const std::string array_literal_example = "[SIZE] Type {values...}";
inline const std::string array_type_example = "[SIZE] Type";
inline const std::string array_literal_first_symbol = array_literal_example.substr(0, 1);
inline const std::string array_type_first_symbol = array_type_example.substr(0, 1);

inline const std::string boolean_not_operator = "!";
inline const std::string minus_sign_prefix = "-";
inline const std::string plus_sign_prefix = "+";
inline const std::string math_decrement_operator = "--";
inline const std::string math_increment_operator = "++";

inline const std::string boolean_and_operator = "&&";
inline const std::string boolean_or_operator = "||";
inline const std::string boolean_xor_operator = "^^";

inline const std::string string_soft_concatenation_operator = "+";
inline const std::string string_hard_concatenation_operator = "*";

inline const std::string math_add_operator = "+";
inline const std::string math_sub_operator = "-";
inline const std::string math_mul_operator = "*";
inline const std::string math_div_operator = "/";
inline const std::string math_mod_operator = "%";
inline const std::string math_pow_operator = "^";

inline const std::string boolean_less_than_operator = "<";
inline const std::string boolean_greater_than_operator = ">";
inline const std::string boolean_less_than_or_equal_operator = "<=";
inline const std::string boolean_greater_than_or_equal_operator = ">=";
inline const std::string boolean_equal_operator = "==";
inline const std::string boolean_not_equal_operator = "!=";

inline const std::string boolean_true_literal = "true";
inline const std::string boolean_false_literal = "false";

inline const std::set<char> symbols{
    '+','-','*','/','=','.',':','<','>','!','(',')',
    '[',']','{','}',',',';','&','|','^','#','%','$','@'
};

inline const std::set<std::string> combinations{
    ">=", "<=", "!=", "++", "--", "==", "->", "<-", "&&", "||", "^^", "::"
};

inline const std::set<char> string_opening_characters {
    '\'','"','`'
};

inline const std::set<char> discardable{
    '\t','\r','\0','\n',' '
};

inline const std::set<std::string> prefixes{
    "!","-","+","#",".","&"
};

inline const std::set<std::string> infixies{
    "+","-","*","/","%","^",
    "<",">","<=",">=","==","!=",
    "&&","||","^^"
};

inline const std::map<std::string, short> infix_operators_priority {
    { "||", 1 }, { "&&", 1 }, { "^^", 1 }, { "<", 2 }, { ">", 2 }, { "<=", 2 }, { ">=", 2 }, 
    { "!=", 2 }, { "==", 2 }, { "+", 3 }, { "-", 3 }, { "*", 4 }, { "/", 4 }, { "%", 4 }, 
    { "^", 6 }, { square_brackets_access, 8}, { ".", 9 } 
};

inline const std::map<std::string, short> prefix_operators_priority {
    { "!", 5 }, { "-", 5 }, { "+", 5 }, { pointer_dereference_operator, 7 }, { address_operator, 7 }
};

inline const std::map<std::string, Token::Type> keywords {
    {"func",     Token::Type::func_keyword},     {"struct",   Token::Type::struct_keyword},
    {"union",    Token::Type::union_keyword},    {"enum",     Token::Type::enum_keyword},
    {"return",   Token::Type::return_keyword},   {"break",    Token::Type::break_keyword},
    {"continue", Token::Type::continue_keyword}, {"defer",    Token::Type::defer_keyword},
    {"throw",    Token::Type::throw_keyword},    {"try",      Token::Type::try_keyword},
    {"catch",    Token::Type::catch_keyword},    {"attempt",  Token::Type::attempt_keyword},
    {"if",       Token::Type::if_keyword},       {"else",     Token::Type::else_keyword},
    {"while",    Token::Type::while_keyword},    {"until",    Token::Type::until_keyword},
    {"var",      Token::Type::var_keyword},      {"const",    Token::Type::const_keyword}
};

inline const std::set<std::string> primitive_types {
    "Int", "Float", "Bool",
    "String", "Char"
};