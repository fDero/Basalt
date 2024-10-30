//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "misc/file_extensions.hpp"
#include "errors/commandline_errors.hpp"

[[nodiscard]] bool check_for_specific_file_extension(const std::string& filename, const std::string& extension) {
    const size_t size = filename.size();
    const size_t extension_size = extension.size();
    if (size < extension_size) {
        return false;
    }
    const size_t prefix_size = size - extension_size;
    return (filename.substr(prefix_size, extension_size) == extension);
}

[[nodiscard]] FileExtension extract_file_extension(const std::string& filename) {
    if (check_for_specific_file_extension(filename, ".basalt")) return FileExtension::basalt;
    if (check_for_specific_file_extension(filename, ".bt"))     return FileExtension::basalt;
    if (check_for_specific_file_extension(filename, ".elf"))    return FileExtension::elf;
    if (check_for_specific_file_extension(filename, ".ll"))     return FileExtension::ll;
    if (check_for_specific_file_extension(filename, ".exe"))    return FileExtension::exe;
    throw_unrecognized_file_extension(filename);
}