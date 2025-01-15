//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "frontend/tokenizer.hpp"

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

inline const std::set<std::string> infixies {
    "+","-","*","/","%",
    "<",">","<=",">=","==","!=",
    "&&","||","^^"
};

enum class OperatorKind {
    less_then,
    greater_then,
    less_then_or_equal,
    greater_then_or_equal,
    equal,
    not_equal,
    address_of,
    pointer_dereference_op,
    boolean_not,
    math_increment,
    math_decrement,
    and_operator,
    or_operator,
    xor_operator,
    plus_operator,
    minus_operator,
    mul_operator,
    div_operator,
    mod_operator,
    is_operator,
    as_operator
};

inline const std::map<std::string, OperatorKind> operator_kinds {
    { "<" ,                         OperatorKind::less_then                 },
    { ">" ,                         OperatorKind::greater_then              },
    { "<=" ,                        OperatorKind::less_then_or_equal        },
    { ">= ",                        OperatorKind::greater_then_or_equal     },
    { "==",                         OperatorKind::equal                     },
    { "!=",                         OperatorKind::not_equal                 },
    { address_operator,             OperatorKind::address_of                },
    { pointer_dereference_operator, OperatorKind::pointer_dereference_op    },
    { "!",                          OperatorKind::boolean_not               },
    { "++",                         OperatorKind::math_increment            },
    { "--",                         OperatorKind::math_decrement            },
    { "&&",                         OperatorKind::and_operator              },
    { "||",                         OperatorKind::or_operator               },
    { "^^",                         OperatorKind::xor_operator              },
    { "+",                          OperatorKind::plus_operator             },
    { "-",                          OperatorKind::minus_operator            },
    { "*",                          OperatorKind::mul_operator              },
    { "/",                          OperatorKind::div_operator              },
    { "%",                          OperatorKind::mod_operator              },
    { "is",                         OperatorKind::is_operator               },
    { "as",                         OperatorKind::as_operator               }
};

inline const std::set<std::string> type_operators { 
    "is", "as" 
};

inline const std::map<std::string, short> infix_operators_priority {
    { "||", 1 }, { "&&", 1 }, { "^^", 1 }, { "<", 2 }, { ">", 2 }, { "<=", 2 }, { ">=", 2 }, 
    { "!=", 2 }, { "==", 2 }, { "+", 3 }, { "-", 3 }, { "*", 4 }, { "/", 4 }, { "%", 4 }
};

inline const std::map<std::string, short> prefix_operators_priority {
    { "!", 5 }, { "-", 5 }, { "+", 5 }, { "++", 5 }, { "--", 5 },
    { pointer_dereference_operator, 7 }, { address_operator, 7 }
};

inline const std::map<std::string, Token::Type> keywords {
    {"func",     Token::Type::func_keyword},     
    {"struct",   Token::Type::struct_keyword},
    {"union",    Token::Type::union_keyword},    
    {"return",   Token::Type::return_keyword},   
    {"break",    Token::Type::break_keyword},
    {"continue", Token::Type::continue_keyword}, 
    {"alias",    Token::Type::alias_keyword},
    {"if",       Token::Type::if_keyword},       
    {"else",     Token::Type::else_keyword},
    {"while",    Token::Type::while_keyword},    
    {"until",    Token::Type::until_keyword},
    {"var",      Token::Type::var_keyword},      
    {"const",    Token::Type::const_keyword},
    {"is",       Token::Type::is_keyword},       
    {"as",       Token::Type::as_keyword},
    {"package",  Token::Type::package_keyword},  
    {"import",   Token::Type::import_keyword}
};

inline const std::string constexpr int_type        = "Int";
inline const std::string constexpr float_type      = "Float";
inline const std::string constexpr bool_type       = "Bool";
inline const std::string constexpr char_type       = "Char";
inline const std::string constexpr string_type     = "String";
inline const std::string constexpr raw_string_type = "RawString";

inline const std::set<std::string> primitive_types {
    int_type,
    float_type,
    bool_type,
    char_type,
    string_type,
    raw_string_type
};