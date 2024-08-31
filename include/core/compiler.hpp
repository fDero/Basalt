/**
 * @file compiler.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the Compiler class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <vector>
#include <string>

#include "model/program_representation.hpp"
#include "preprocessing/preprocessor.hpp"

/**
 * @brief   Just a simple class that exposes all the necessary methods to compile a program.
 *          It is used to simplify the API and to provide a clear separation of concerns.
 *      
 * @details The Compiler class is used to compile a program. It is used to open source files,
 *          tokenize them, parse them, and then perform static analysis and code generation.
 *          Everything this class does is to delegate the work to other classes, and to provide
 *          a simple API to the caller.
 * 
 * @see     Tokenizer
 * @see     Parser
 * @see     FileRepresentation
 * @see     ProgramRepresentation
 * @see     PreProcessor
 * 
 */
class Compiler {

    public:
        Compiler(
            const std::vector<std::string>& source_files, 
            const std::vector<std::string>& output_files
        );

        void perform_static_analysis();
        void perform_code_generation();
        void output_the_required_files();

    private:
        ProgramRepresentation program_representation;
        PreProcessor preprocessor;
        
        const std::vector<std::string>& output_files;
};