//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>


struct FileDescriptor {
    enum class FileExtension {
        basalt,
        llvm,
        assembly,
        object,
        static_library,
        dynamic_library
    };

    explicit FileDescriptor(
        std::string fullpath
    );

    std::string fullpath;
    std::string basename;
    FileExtension extension;
};
