//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>
#include <utility>

#include "frontend/file_descriptor.hpp"
#include "errors/commandline_errors.hpp"

using FileExtension = FileDescriptor::FileExtension;


bool check_extension(const std::string& filename, const std::string& extension) {
    const size_t size = filename.size();
    const size_t extension_size = extension.size();
    if (size < extension_size) {
        return false;
    }
    const size_t prefix_size = size - extension_size;
    return (filename.substr(prefix_size, extension_size) == extension);
}

FileDescriptor::FileExtension extract_file_extension(const std::string& filename) {
    if (check_extension(filename, ".basalt")) return FileExtension::basalt;
    if (check_extension(filename, ".bt"))     return FileExtension::basalt;
    if (check_extension(filename, ".llvm"))   return FileExtension::llvm;
    if (check_extension(filename, ".ll"))     return FileExtension::llvm;
    if (check_extension(filename, ".asm"))    return FileExtension::assembly;
    if (check_extension(filename, ".s"))      return FileExtension::assembly;
    if (check_extension(filename, ".obj"))    return FileExtension::object;
    if (check_extension(filename, ".o"))      return FileExtension::object;
    if (check_extension(filename, ".a"))      return FileExtension::static_library;
    if (check_extension(filename, ".lib"))    return FileExtension::static_library;
    if (check_extension(filename, ".so"))     return FileExtension::dynamic_library;
    if (check_extension(filename, ".dylib"))  return FileExtension::dynamic_library;
    if (check_extension(filename, ".dll"))    return FileExtension::dynamic_library;
    throw_unrecognized_file_extension(filename);
}

std::string extract_basename(const std::string& fullname) {
    const size_t last_separator = fullname.find_last_of("/\\");
    if (last_separator == std::string::npos) {
        return fullname;
    }
    return fullname.substr(last_separator + 1);
}

FileDescriptor::FileDescriptor(std::string fullpath)
    : fullpath(std::move(fullpath))
    , basename(extract_basename(this->fullpath))
    , extension(extract_file_extension(this->fullpath))
{}
