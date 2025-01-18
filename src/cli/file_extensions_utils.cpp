//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "cli/file_extensions.hpp"
#include "errors/commandline_errors.hpp"

bool check_for_specific_file_extension(const std::string& filename, const std::string& extension) {
    const size_t size = filename.size();
    const size_t extension_size = extension.size();
    if (size < extension_size) {
        return false;
    }
    const size_t prefix_size = size - extension_size;
    return (filename.substr(prefix_size, extension_size) == extension);
}

FileExtension extract_file_extension(const std::string& filename) {
    if (check_for_specific_file_extension(filename, ".basalt")) return FileExtension::basalt;
    if (check_for_specific_file_extension(filename, ".bt"))     return FileExtension::basalt;
    if (check_for_specific_file_extension(filename, ".llvm"))   return FileExtension::llvm;
    if (check_for_specific_file_extension(filename, ".ll"))     return FileExtension::llvm;
    if (check_for_specific_file_extension(filename, ".asm"))    return FileExtension::assembly;
    if (check_for_specific_file_extension(filename, ".s"))      return FileExtension::assembly;
    if (check_for_specific_file_extension(filename, ".obj"))    return FileExtension::object;
    if (check_for_specific_file_extension(filename, ".o"))      return FileExtension::object;
    throw_unrecognized_file_extension(filename);
}