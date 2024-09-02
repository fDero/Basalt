/**
 * @file file_representation.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the FileRepresentation struct
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string>
#include <vector>

#include "language/definitions.hpp"

/**
 * @brief   This struct represents a file in the program, it is the output
 *          of the parsing phase. 
 * 
 * @details This struct represents a file in the program, it is the output
 *          of the parsing phase. It contains the metadata of the file, such as
 *          the filename, the package name and the imports. It also contains
 *          the type definitions and the function definitions contained in such file.
 * 
 * @note    Every Defintion is to be considered as
 *          the root of an AST.
 */
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