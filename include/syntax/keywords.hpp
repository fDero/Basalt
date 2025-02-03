//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <map>

#include "frontend/tokenizer.hpp"

inline const std::string constexpr single_line_comment       = "//";
inline const std::string constexpr multiline_comment_opening = "/*";
inline const std::string constexpr multiline_comment_closing = "*/";
inline const std::string constexpr namespace_concatenation   = "::";

inline const std::string constexpr extern_keyword  = "extern";
inline const std::string constexpr func_keyword    = "func";
inline const std::string constexpr struct_keyword  = "strcut";
inline const std::string constexpr union_keyword   = "union";
inline const std::string constexpr alias_keyword   = "alias";

inline const std::string constexpr boolean_true_literal  = "true";
inline const std::string constexpr boolean_false_literal = "false";

inline const std::map<std::string, Token::Type> keywords {
    { extern_keyword,   Token::Type::extern_keyword   },
    { func_keyword,     Token::Type::func_keyword     },
    { struct_keyword,   Token::Type::struct_keyword   },
    { union_keyword,    Token::Type::union_keyword    },    
    { alias_keyword,    Token::Type::alias_keyword    },
    { "package",        Token::Type::package_keyword  },  
    { "import",         Token::Type::import_keyword   },
    { "return",         Token::Type::return_keyword   },   
    { "is",             Token::Type::is_keyword       },       
    { "as",             Token::Type::as_keyword       },
    { "break",          Token::Type::break_keyword    },
    { "continue",       Token::Type::continue_keyword }, 
    { "if",             Token::Type::if_keyword       },       
    { "else",           Token::Type::else_keyword     },
    { "while",          Token::Type::while_keyword    },    
    { "until",          Token::Type::until_keyword    },
    { "var",            Token::Type::var_keyword      },      
    { "const",          Token::Type::const_keyword    },
};