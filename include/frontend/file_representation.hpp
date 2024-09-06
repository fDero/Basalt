
#pragma once

#include <string>
#include <vector>

#include "language/definitions.hpp"

struct FileRepresentation {

    struct Metadata {
        std::string filename;
        std::string packagename;
        std::vector<std::string> imports;
    };

    Metadata file_metadata;
    std::vector<TypeDefinition> type_defs;
    std::vector<FunctionDefinition> func_defs;
};