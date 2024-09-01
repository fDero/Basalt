/**
 * @file debug_informations_aware_entity.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the DebugInformationsAwareEntity struct
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <cstdint>

/**
 * @brief   Used to represent an entity that is aware of its position in the source-code file.
 * 
 * @details The DebugInformationsAwareEntity struct is used to represent an entity that is aware of its position in 
 *          the source-code file in terms of filename, line number, token number, and character position. This struct
 *          is extended by most of the entities in the language module, definitions, statements, expressions etc...
 * 
 */
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

    DebugInformationsAwareEntity(const DebugInformationsAwareEntity& other) = default;
    DebugInformationsAwareEntity& operator=(const DebugInformationsAwareEntity& other) = default;

    DebugInformationsAwareEntity(DebugInformationsAwareEntity&& other) = default;
    DebugInformationsAwareEntity& operator=(DebugInformationsAwareEntity&& other) = default;

    const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const { return *this; }
};
