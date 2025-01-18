//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "cli/commandline.hpp"
#include "errors/error-types.hpp"

[[noreturn]] void throw_invalid_output_file_format(const std::string& filename);

[[noreturn]] void throw_invalid_input_file_format(const std::string& filename);

[[noreturn]] void throw_unrecognized_flag(char** argv, int i);

[[noreturn]] void throw_basalt_used_as_output(const std::string& filename);

[[noreturn]] void throw_unspecified_commandline_subcommand();

[[noreturn]] void throw_unrecognized_file_extension(const std::string& filename);

void avoid_lack_of_input_files(const std::vector<std::string>& input_files);

void avoid_lack_of_output_files(const std::vector<std::string>& output_files);

void avoid_duplicate_input_files(std::vector<std::string>& input_files);

void avoid_duplicate_output_file_extensions(std::vector<FileExtension> extensions);

void avoid_lack_of_target_triple(const std::optional<std::string>& target_triple);

void ensure_lack_of_target_triple(const std::optional<std::string>& target_triple);

void ensure_input_files_exist(std::vector<std::string>& input_files);

void ensure_lack_of_output_files(const std::vector<std::string>& output_files);

void ensure_lack_of_input_files(const std::vector<std::string>& input_files);

void ensure_source_file_is_open(const std::fstream& input_file, const std::string& file_name);

void ensure_valid_target_triple(const std::string& target_triple, const std::string& error);

