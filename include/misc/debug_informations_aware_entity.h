
#pragma once

struct DebugInformationsAwareEntity {

    std::string filename;
    unsigned long line_number;
    unsigned int tok_number;
    unsigned int char_pos;

    void source_informations_from(const DebugInformationsAwareEntity& other){
        filename = other.filename;
        line_number = other.line_number;
        tok_number = other.tok_number;
        char_pos = other.char_pos;
    }

    DebugInformationsAwareEntity(
        const std::string& filename,
        unsigned long line_number,
        unsigned int tok_number,
        unsigned int char_pos
    )
        : filename(filename)
        , line_number(line_number)
        , tok_number(tok_number)
        , char_pos(char_pos)
    {}

    DebugInformationsAwareEntity(const DebugInformationsAwareEntity& other) = default;
};