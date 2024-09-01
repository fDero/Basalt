/**
 * @file tokenized_file.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the TokenizedFile struct 
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>

#include "frontend/token.hpp"

/**
 * @brief   Used to represent a file that has been tokenized.
 * 
 * @details The TokenizedFile struct is used to represent a file that has been tokenized by the Tokenizer class.
 *          It contains the filename of the file that has been tokenized and a vector of Token objects, each of which
 *          represents a single unit of text extracted from the source code.
 * 
 * @see     Token
 * @see     Tokenizer
 * 
 */
struct TokenizedFile {
    std::string filename;
    std::vector<Token> tokens;
};