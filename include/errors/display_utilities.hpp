//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "errors/tokenization_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/internal_errors.hpp"
#include "misc/console_colors.hpp"

#include <iostream>

void display_target_line(size_t char_pos, const std::string& line);

void display_error_context(const std::string& filename, size_t target_line_number, size_t char_pos);

void display_commandline_error(const CommandLineError& err);

void display_tokenization_error(const TokenizationError& err);

void display_parsing_error(const ParsingError& err);

void display_internal_error(const InternalError& err);

void display_runtime_error(const std::runtime_error& err);

void display_unrecognized_error();

void display_pretty_error_message(std::exception_ptr& error);