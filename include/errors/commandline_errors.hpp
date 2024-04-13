
#pragma once
#include "toolchain/commandline.hpp"
#include "errors/error-types.hpp"

[[noreturn]] void throw_invalid_output_file_format(const std::string& filename);

[[noreturn]] void throw_invalid_input_file_format(const std::string& filename);

[[noreturn]] void throw_unrecognized_flag(char** argv, int i);

[[noreturn]] void throw_basalt_used_as_output(const std::string& filename);

[[noreturn]] void throw_unspecified_commandline_mode();

[[noreturn]] void throw_unrecognized_file_extension(const std::string& filename);

void avoid_conflicting_commandline_flags(
    CommandLineController::Mode previous, 
    CommandLineController::Mode current
);

void avoid_lack_of_input_files(const std::vector<std::string>& input_files);

void avoid_lack_of_output_files(const std::vector<std::string>& output_files);

void avoid_duplicate_input_files(std::vector<std::string>& input_files);

void avoid_duplicate_output_file_extensions(
    const std::vector<CommandLineController::FileExtension>& already_encountered, 
    const CommandLineController::FileExtension file_ext
);

void ensure_lack_of_input_files(const std::vector<std::string>& input_files);

void ensure_lack_of_output_files(const std::vector<std::string>& output_files);

void ensure_source_file_is_open(const std::fstream& input_file, const std::string& file_name);

void ensure_version_flag_is_the_only_one(
    int current_flag_index, int arg_counter, const std::vector<std::string>& input_files, 
    const std::vector<std::string>& output_files, const CommandLineController::Mode mode
);

void ensure_help_flag_is_the_only_one(
    int current_flag_index, int arg_counter, const std::vector<std::string>& input_files, 
    const std::vector<std::string>& output_files, const CommandLineController::Mode mode
);

