
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
inline const std::string pointer_dereference_operator = "#";
inline const std::string pointer_declaration_symbol = "#";

inline const std::set<char> symbols{
    '+','-','*','/','=','.',':','<','>','!','(',')',
    '[',']','{','}',',',';','&','|','^','#','%'
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
    "!","-","+","#","."
};

inline const std::set<std::string> infixies{
    "+","-","*","/","%","<",">","<=",">=","==","&&","||","^^","!=","^"
};

inline const std::map<std::string, short> infix_operators_priority {
    { "||", 1 }, { "&&", 1 }, { "^^", 1 }, { "<", 2 }, { ">", 2 }, { "<=", 2 }, { ">=", 2 }, 
    { "!=", 2 }, { "==", 2 }, { "+", 3 }, { "-", 3 }, { "*", 4 }, { "/", 4 }, { "%", 4 }, 
    { "^", 6 }, { "[square-brackets-access]", 8}, { ".", 9 } 
};

inline const std::map<std::string, short> prefix_operators_priority {
    { "!", 5 }, { "-", 5 }, { "+", 5 }, { pointer_dereference_operator, 7 }
};

inline const std::map<std::string, Token::Type> keywords {
    {"func",     Token::Type::func_keyword},     {"struct",   Token::Type::struct_keyword},
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