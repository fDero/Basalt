
#pragma once

struct DebugInformationsAwareEntity {

    std::string filename;
    size_t line_number;
    size_t tok_number;
    size_t char_pos;

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
};
