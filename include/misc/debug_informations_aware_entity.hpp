//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <cstdint>

struct DebugInformationsAwareEntity {

    std::string filename;
    size_t line_number;
    size_t tok_number;
    size_t char_pos;

    virtual ~DebugInformationsAwareEntity() = default;

    DebugInformationsAwareEntity(
        const std::string& filename,
        size_t line_number,
        size_t tok_number,
        size_t char_pos
    )
        : filename(filename)
        , line_number(line_number)
        , tok_number(tok_number)
        , char_pos(char_pos)
    {}

    std::string unique_string_id() const {
        return filename 
            + ":" + std::to_string(line_number) 
            + ":" + std::to_string(tok_number) 
            + ":" + std::to_string(char_pos);
    }

    DebugInformationsAwareEntity(const DebugInformationsAwareEntity& other) = default;
    DebugInformationsAwareEntity& operator=(const DebugInformationsAwareEntity& other) = default;

    DebugInformationsAwareEntity(DebugInformationsAwareEntity&& other) = default;
    DebugInformationsAwareEntity& operator=(DebugInformationsAwareEntity&& other) = default;

    const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const { return *this; }
};
