//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <map>

#include "frontend/tokenizer.hpp"

inline const std::string constexpr extern_keyword            = "extern";
inline const std::string constexpr func_keyword              = "func";
inline const std::string constexpr struct_keyword            = "struct";
inline const std::string constexpr union_keyword             = "union";
inline const std::string constexpr alias_keyword             = "alias";

inline const std::string constexpr package_keyword           = "package";
inline const std::string constexpr import_keyword            = "import";

inline const std::string constexpr return_keyword            = "return";
inline const std::string constexpr break_keyword             = "break";
inline const std::string constexpr continue_keyword          = "continue";

inline const std::string constexpr if_keyword                = "if";
inline const std::string constexpr else_keyword              = "else";
inline const std::string constexpr while_keyword             = "while";
inline const std::string constexpr until_keyword             = "until";

inline const std::string constexpr is_keyword                = "is";
inline const std::string constexpr as_keyword                = "as";

inline const std::string constexpr var_keyword               = "var";
inline const std::string constexpr const_keyword             = "const";

inline const std::string constexpr boolean_true_literal      = "true";
inline const std::string constexpr boolean_false_literal     = "false";

inline const std::map<std::string, Token::Type> keywords {
    { extern_keyword,   Token::Type::extern_keyword   },
    { func_keyword,     Token::Type::func_keyword     },
    { struct_keyword,   Token::Type::struct_keyword   },
    { union_keyword,    Token::Type::union_keyword    },    
    { alias_keyword,    Token::Type::alias_keyword    },
    { package_keyword,  Token::Type::package_keyword  },  
    { import_keyword,   Token::Type::import_keyword   },
    { return_keyword,   Token::Type::return_keyword   },   
    { break_keyword,    Token::Type::break_keyword    },
    { continue_keyword, Token::Type::continue_keyword }, 
    { if_keyword,       Token::Type::if_keyword       },       
    { else_keyword,     Token::Type::else_keyword     },
    { while_keyword,    Token::Type::while_keyword    },    
    { until_keyword,    Token::Type::until_keyword    },
    { is_keyword,       Token::Type::is_keyword       },       
    { as_keyword,       Token::Type::as_keyword       },
    { var_keyword,      Token::Type::var_keyword      },      
    { const_keyword,    Token::Type::const_keyword    },
};