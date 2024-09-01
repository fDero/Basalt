/**
 * @file token.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the Token struct
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

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

/**
 * @brief   Used to represent a single unit of text extracted from the source code.
 * 
 * @details The Token struct is used to represent a single unit of text extracted from the source code,
 *          it extends the DebugInformationsAwareEntity, hence it keeps track of the position of the token
 *          in the source code file, in terms of filepath, line number, token number, and character position.
 *          The Token struct also keeps track of the type of the token, which can be later use by the parser
 *          to understand the meaning of the token faster then it would be possible by just looking at the text.
 * 
 * @see     DebugInformationsAwareEntity
 * @see     Tokenizer
 * 
 */
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