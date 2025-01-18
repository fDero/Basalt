//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <exception>
#include <string>

#include "errors/display_utilities.hpp"
#include "misc/console_colors.hpp"

void display_target_line(size_t char_pos, const std::string& line) {
    std::cout << "\t";
    for (size_t i = 0; i < line.size(); i++) {
        if(i != char_pos) std::cout << yellow(line.substr(i,1)); 
        else std::cout << red(line.substr(i,1));
        char_pos += (isalnum(line[i]) && i == char_pos); 
    }
    std::cout << "\n";
}

void display_error_context(const std::string& filename, size_t target_line_number, size_t char_pos) {
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

void display_commandline_error(const CommandLineError& err) {
    std::cout << std::endl << bold_red("COMMANDLINE ERROR: ") << red(err.error_message + "\n\n")
    << purple("type 'basalt help' in your console to get more info\n\n");
}

void display_tokenization_error(const TokenizationError& err) {
    std::cout << std::endl
    << bold_red("TOKENIZATION ERROR: ") << red(err.error_message) << "\n\n";
    display_error_context(err.filename, err.line_number, err.char_pos);
    std::cout << purple("\n in file: ") << err.filename << " "
    << purple("at line: ") << err.line_number << "\n\n";
}

void display_parsing_error(const ParsingError& err) {
    std::cout << std::endl
    << bold_red("SYNTAX ERROR: ") << red(err.error_message) << "\n\n";
    display_error_context(err.filename, err.line_number, err.char_pos);
    std::cout << purple("\n in file: ") << err.filename
    << purple(" at line: ") << err.line_number << "\n\n";
}

void display_internal_error(const InternalError& err) {
    std::cout << std::endl
    << bold_red("INTERNAL COMPILER ERROR: ") << red(err.error_message + "\n\n")
    << yellow("please, write an extensive bug report explaining the issue and report it on the github page ") 
    << yellow("of this project at https://www.github.com/fDero/Basalt\n\n");
}

void display_runtime_error(const std::runtime_error& err) {
    std::cout << std::endl
    << bold_red("RUNTIME ERROR: ") << red(err.what())
    << blue("\n\nbetter error messages are work in progress right now\n\n");
}

void display_unrecognized_error() {
    InternalError unrecognized_error { "an unrecognized error has occurred, the point of failure is unknown" };
    display_internal_error(unrecognized_error);
}

void display_pretty_error_message(std::exception_ptr& error) {
    try {
        std::rethrow_exception(error);
    }
    catch (const CommandLineError& err)    { display_commandline_error(err);  }
    catch (const TokenizationError& err)   { display_tokenization_error(err); }
    catch (const ParsingError& err)        { display_parsing_error(err);      }
    catch (const InternalError& err)       { display_internal_error(err);     }
    catch (const std::runtime_error& err)  { display_runtime_error(err);      }
    catch (...)                            { display_unrecognized_error();    }
}