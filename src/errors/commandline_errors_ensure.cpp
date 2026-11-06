//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <fstream>
#include <ios>
#include <string>
#include <vector>

#include "frontend/file_descriptor.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/compilation_error.hpp"

using FileExtension = FileDescriptor::FileExtension;


void ensure_input_files_exist(const std::vector<std::string>& input_files) {
    for (const std::string& input_file_name : input_files) {
        std::fstream fstream_file_reader(input_file_name, std::ios::in);
        ensure_source_file_is_open(fstream_file_reader, input_file_name);
    }
}

void ensure_lack_of_target_triple(const std::vector<std::string>& target_triples) {
    if (!target_triples.empty()) {
        throw CommandLineError(
            "target triple was given but it wasn't needed, \n"
            "(please run `basalt help` for more info)"
        );
    }
}

void ensure_lack_of_link_files(const std::vector<std::string>& link_files) {
    if (!link_files.empty()) {
        throw CommandLineError(
            "libraries to link were given but they weren't needed, \n"
            "(please run `basalt help` for more info)"
        );
    }
}

void ensure_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (!output_files.empty()) {
        throw CommandLineError(
            "output files were given but they weren't needed, \n"
            "(please run `basalt help` for more info)"
        );
    }
}

void ensure_lack_of_input_files(const std::vector<std::string>& input_files) {
    if (!input_files.empty()) {
        throw CommandLineError(
            "input files were given but they weren't needed,  \n"
            "(please run `basalt help` for more info) \n"
        );
    }
}

void ensure_files_are_basalt_sources(const std::vector<FileDescriptor>& files) {
    for (const FileDescriptor& file : files) {
        if (file.extension != FileExtension::basalt) {
            throw_invalid_input_file_format(file.fullpath);
        }
    }
}

void ensure_files_are_compilation_outputs(const std::vector<FileDescriptor>& files) {
    for (const FileDescriptor& file : files) {
        switch (file.extension) {
            case FileExtension::llvm:     continue;
            case FileExtension::assembly: continue;
            case FileExtension::object:   continue;
            case FileExtension::basalt:   throw_basalt_used_as_output(file.fullpath);
            default:                      throw_invalid_output_file_format(file.fullpath);
        }
    }
}

void ensure_files_are_libraries(const std::vector<FileDescriptor>& files) {
    for (const FileDescriptor& file : files) {
        if (file.extension != FileExtension::static_library && file.extension != FileExtension::dynamic_library) {
            throw_invalid_library_file_format(file.fullpath);
        }
    }
}

void ensure_source_file_is_open(const std::fstream& input_file, const std::string& file_name) {
    if (!input_file.is_open()) {
        throw CommandLineError(
            "file " + file_name + " either is missing or is inaccessible \n"
            "(please double-check the name of the file and make sure the \n"
            "path to that file is fully and correctly specified)"
        );
    }
}

void ensure_valid_target_triple(const std::string& target_triple, const std::string& error) {
    if (!error.empty()) {
        throw CommandLineError(
            "target triple " + target_triple + " is invalid:\n" + error
        );
    }
}
