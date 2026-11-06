//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "frontend/file_descriptor.hpp"


[[noreturn]] void throw_invalid_output_file_format(const std::string& filename);

[[noreturn]] void throw_invalid_input_file_format(const std::string& filename);

[[noreturn]] void throw_invalid_library_file_format(const std::string& filename);

[[noreturn]] void throw_unrecognized_flag(char** argv, int i);

[[noreturn]] void throw_unrecognized_flag(const std::string& flag);

[[noreturn]] void throw_basalt_used_as_output(const std::string& filename);

[[noreturn]] void throw_unspecified_commandline_subcommand();

[[noreturn]] void throw_unrecognized_file_extension(const std::string& filename);

[[noreturn]] void throw_unrecognized_file_extension(const FileDescriptor& file);

void avoid_lack_of_input_files(const std::vector<std::string>& input_files);

void avoid_lack_of_output_files(const std::vector<std::string>& output_files);

void avoid_duplicate_input_files(std::vector<std::string> input_files);

void avoid_duplicate_output_file_extensions(const std::vector<FileDescriptor>& output_files);

void avoid_multiple_target_triples(const std::vector<std::string>& target_triples);

void ensure_lack_of_target_triple(const std::vector<std::string>& target_triples);

void ensure_lack_of_link_files(const std::vector<std::string>& link_files);

void ensure_lack_of_output_files(const std::vector<std::string>& output_files);

void ensure_lack_of_input_files(const std::vector<std::string>& input_files);

void ensure_input_files_exist(const std::vector<std::string>& input_files);

void ensure_files_are_basalt_sources(const std::vector<FileDescriptor>& files);

void ensure_files_are_compilation_outputs(const std::vector<FileDescriptor>& files);

void ensure_files_are_libraries(const std::vector<FileDescriptor>& files);

void ensure_source_file_is_open(const std::fstream& input_file, const std::string& file_name);

void ensure_valid_target_triple(const std::string& target_triple, const std::string& error);
