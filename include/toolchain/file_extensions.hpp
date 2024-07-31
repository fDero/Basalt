
#pragma once
#include <string>

enum class FileExtension {
    basalt, 
    ll, 
    elf, 
    exe
};

[[nodiscard]] FileExtension extract_file_extension(const std::string& filename);