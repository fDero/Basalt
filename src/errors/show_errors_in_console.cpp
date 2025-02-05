//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <exception>
#include <string>
#include <iostream>
#include <fstream>

#include "errors/compilation_error.hpp"
#include "misc/console_colors.hpp"

using CompilationError::Kind::CommandLineError;
using CompilationError::Kind::TokenizationError;
using CompilationError::Kind::ParsingError;
using CompilationError::Kind::IndexingError;
using CompilationError::Kind::ValidationError;
using CompilationError::Kind::InternalError;

static void display_target_line(size_t char_pos, const std::string& line) {
    std::cout << "\t";
    for (size_t i = 0; i < line.size(); i++) {
        if(i != char_pos) std::cout << yellow(line.substr(i,1)); 
        else std::cout << red(line.substr(i,1));
        char_pos += (isalnum(line[i]) && i == char_pos); 
    }
    std::cout << "\n";
}

static void display_error_context(const std::string& filename, size_t target_line_number, size_t char_pos) {
    std::ifstream infile(filename);
    std::string line;
    int spread = 4;
    for (size_t line_counter = 1; std::getline(infile, line); ++line_counter) {
        bool is_after_start = (line_counter > target_line_number - spread);
        bool is_before_finish = (line_counter < target_line_number + spread);
        bool is_at_target_spot = (line_counter == target_line_number);
        if (is_at_target_spot) display_target_line(char_pos, line);
        else if (is_after_start && is_before_finish) std::cout << "\t" << line << "\n";
        else if (line_counter > target_line_number + spread) break;
    }
}

static void display_coordinates(const DebugInformationsAwareEntity& debug_info) {
    std::cout 
        << purple("at line: ") 
        << debug_info.line_number 
        << purple(" at char: ") 
        << debug_info.char_pos 
        << std::endl
        << std::endl;
}

static void display_error_kind(CompilationError::Kind kind) {
    std::cout << std::endl << std::endl; 
    switch (kind) {
        break; case CommandLineError:  std::cout << bold_red("COMMAND LINE ERROR: ");
        break; case TokenizationError: std::cout << bold_red("TOKENIZATION ERROR: ");
        break; case ParsingError:      std::cout << bold_red("SYNTAX ERROR: ");
        break; case IndexingError:     std::cout << bold_red("INDEXING ERROR: ");
        break; case ValidationError:   std::cout << bold_red("VALIDATION ERROR: ");
        break; case InternalError:     std::cout << bold_red("INTERNAL ERROR: ");
    }
}

static void display_error_message(const std::string& error_message) {
    std::cout 
        << red(error_message) 
        << std::endl 
        << std::endl;
}

static void display_runtime_error(const std::runtime_error& err) {
    std::cout << std::endl
    << bold_red("RUNTIME ERROR: ") 
    << red(err.what())
    << std::endl
    << std::endl
    << blue("better error messages are work in progress right now")
    << std::endl
    << std::endl;
}

static void display_unrecognized_error() {
    display_error_kind(InternalError);
    display_error_message(
        "an unrecognized error has occurred, the point of failure is unknown"
    );
}

void display_pretty_error_message(std::exception_ptr& error) {
    try {
        std::rethrow_exception(error);
    }
    catch (const CompilationError& err)    { err.display_pretty();         }
    catch (const std::runtime_error& err)  { display_runtime_error(err);   }
    catch (...)                            { display_unrecognized_error(); }
}

void CompilationError::display_pretty() const {
    display_error_kind(kind);
    display_error_message(std::runtime_error::what());
    if (optional<DebugInformationsAwareEntity>::has_value()) {
        display_error_context(value().filename, value().line_number, value().char_pos);
        display_coordinates(value());
    }
}