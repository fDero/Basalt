//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#ifdef _WIN32
    #include <io.h>
    #define BASALT_ISATTY _isatty
    #define BASALT_FILENO _fileno
#else
    #include <unistd.h>
    #define BASALT_ISATTY isatty
    #define BASALT_FILENO fileno
#endif

#include "errors/compilation_error.hpp"
#include "misc/console_colors.hpp"
#include "misc/debug_informations_aware_entity.hpp"


using Kind = CompilationError::Kind;

static bool colors_enabled() {
    static const bool enabled = std::getenv("NO_COLOR") == nullptr
        && BASALT_ISATTY(BASALT_FILENO(stderr));
    return enabled;
}

using Colorizer = std::string(*)(const std::string&);

static std::string style(Colorizer colorizer, const std::string& text) {
    return colors_enabled() ? colorizer(text) : text;
}

static std::string error_label(Kind kind) {
    switch (kind) {
        case Kind::TokenizationError: return "syntax error";
        case Kind::ParsingError:      return "syntax error";
        case Kind::IndexingError:     return "error";
        case Kind::ValidationError:   return "error";
        case Kind::CommandLineError:  return "command line error";
        case Kind::InternalError:     return "internal compiler error";
    }
    return "error";
}

static bool is_part_of_a_word(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

static void display_source_snippet(const DebugInformationsAwareEntity& location) {
    std::ifstream source_file(location.filename);
    std::string line;
    for (size_t line_counter = 1; std::getline(source_file, line); ++line_counter) {
        if (line_counter != location.line_number) {
            continue;
        }
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        const std::string line_number = std::to_string(location.line_number);
        const std::string gutter(line_number.size(), ' ');
        // underline the whole word (or number, decimal point included) starting at the error position
        size_t underline_length = 1;
        if (location.char_pos < line.size() && is_part_of_a_word(line[location.char_pos])) {
            bool is_number = std::isdigit(static_cast<unsigned char>(line[location.char_pos]));
            auto continues_the_word = [&](char c) { return is_part_of_a_word(c) || (is_number && c == '.'); };
            while (location.char_pos + underline_length < line.size() && continues_the_word(line[location.char_pos + underline_length])) {
                underline_length++;
            }
        }
        // keep tabs as tabs, so the caret lines up with the source line
        std::string caret_offset;
        for (size_t i = 0; i < location.char_pos && i < line.size(); i++) {
            caret_offset += (line[i] == '\t') ? '\t' : ' ';
        }
        if (location.char_pos > line.size()) {
            caret_offset += std::string(location.char_pos - line.size(), ' ');
        }
        std::cerr << " " << style(bold_blue, line_number + " |") << " " << line << "\n";
        std::cerr << " " << style(bold_blue, gutter + " |") << " " << caret_offset
                  << style(bold_red, std::string(underline_length, '^')) << "\n";
        return;
    }
}

static void display_error(
    CompilationError::Kind kind,
    const std::string& message,
    const std::optional<DebugInformationsAwareEntity>& location,
    const std::string& note
) {
    if (location.has_value()) {
        std::cerr << style(bold_gray,
            location->filename + ":" + std::to_string(location->line_number) +
            ":" + std::to_string(location->char_pos + 1) + ": "
        );
    }
    std::cerr << style(bold_red, error_label(kind) + ":") << " " << message << "\n";
    if (location.has_value()) {
        display_source_snippet(location.value());
    }
    if (!note.empty()) {
        std::cerr << style(bold_aqua, "note:") << " " << note << "\n";
    }
    if (kind == Kind::InternalError) {
        std::cerr << style(bold_aqua, "note:") << " this is a bug in the basalt compiler, not in your program; "
            "please report it at https://github.com/fDero/Basalt/issues\n";
    }
}

void display_pretty_error_message(std::exception_ptr& error) {
    try {
        std::rethrow_exception(error);
    }
    catch (const CompilationError& err) {
        err.display_pretty();
    }
    catch (const std::exception& err) {
        display_error(Kind::InternalError, err.what(), std::nullopt, "");
    }
    catch (...) {
        display_error(Kind::InternalError, "an unknown error has occurred", std::nullopt, "");
    }
    std::cerr.flush();
}

void CompilationError::display_pretty() const {
    display_error(kind, std::runtime_error::what(), *this, note);
}
