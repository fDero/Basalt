//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <optional>
#include <stack>
#include <regex>

#include "misc/debug_informations_aware_entity.hpp"

struct Token : public DebugInformationsAwareEntity {

    enum class Type {

        // identifiers, literals, and symbols
        text, 
        type, 
        integer_literal, 
        floating_literal, 
        string_literal, 
        character_literal, 
        boolean_literal,
        symbol, 
        is_keyword, 
        as_keyword,
        
        
        // keywords
        return_keyword, 
        break_keyword, 
        continue_keyword, 
        if_keyword, 
        else_keyword,
        while_keyword, 
        until_keyword, 
        var_keyword, 
        const_keyword, 

        // definitions
        func_keyword, 
        struct_keyword, 
        union_keyword, 
        alias_keyword,

        // meta
        package_keyword, 
        import_keyword, 
        multiline_comment,
    };

    using DebugInformationsAwareEntity::filename;
    using DebugInformationsAwareEntity::line_number;
    using DebugInformationsAwareEntity::tok_number;
    using DebugInformationsAwareEntity::char_pos;
    
    std::string sourcetext;
    Type type;

    Token(
        const std::string& sourcetext,
        const std::string& filename,
        size_t in_line_number,
        size_t in_tok_number,
        size_t in_char_pos,
        Type type
    );

    virtual ~Token() = default;
};