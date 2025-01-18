//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>

#include "errors/commandline_errors.hpp"
#include "cli/commandline.hpp"

void CommandLineController::version_subcommand() {
    ensure_lack_of_output_files(outputs);
    ensure_lack_of_input_files(inputs);
    ensure_lack_of_target_triple(target_triple);


    std::cout << R"(                   ____                   ____                      )" << std::endl;
    std::cout << R"(                  / __ \____ __________ _/ / /_                     )" << std::endl;
    std::cout << R"(                 / __  / __ `/ ___/ __ `/ / __/                     )" << std::endl;
    std::cout << R"(                / /_/ / /_/ (__  ) /_/ / / /_                       )" << std::endl;
    std::cout << R"(               /_____/\__,_/____/\__,_/_/\__/                       )" << std::endl;
    std::cout << R"(                                                                    )" << std::endl;
    std::cout << R"(                                                                    )" << std::endl;
    std::cout << R"(`basalt` is a compiler for the Basalt programming language.         )" << std::endl;
    std::cout << R"(version: 0.0.0 pre-release                                          )" << std::endl;
    std::cout << R"(                                                                    )" << std::endl;
}

void CommandLineController::help_subcommand() {
    ensure_lack_of_output_files(outputs);
    ensure_lack_of_input_files(inputs);
    ensure_lack_of_target_triple(target_triple);
    version_subcommand();
    std::cout << "                                                                       " << std::endl;
    std::cout << "The command line interface (CLI) exposes the following subcommands:    " << std::endl;
    std::cout << "         `help`, `version`, `compile`, `run`, `typecheck`.             " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "Such subcommands can/must be coupled with the following flags to work  " << std::endl;
    std::cout << "properly. The order in which the flags are provided doesn't matter.    " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "               +------------+-------------+-------------+              " << std::endl;
    std::cout << "               | -i --input | -o --output | -t --target |              " << std::endl;
    std::cout << "   +-----------+------------+-------------+-------------+              " << std::endl;
    std::cout << "   | help      |            |             |             |              " << std::endl;
    std::cout << "   +-----------+------------+-------------+-------------+              " << std::endl;
    std::cout << "   | version   |            |             |             |              " << std::endl;
    std::cout << "   +-----------+------------+-------------+-------------+              " << std::endl;
    std::cout << "   | compile   |  REQUIRED  |   REQUIRED  |   ALLOWED   |              " << std::endl;
    std::cout << "   +-----------+------------+-------------+-------------+              " << std::endl;
    std::cout << "   | run       |  REQUIRED  |             |             |              " << std::endl;
    std::cout << "   +-----------+------------+-------------+-------------+              " << std::endl;
    std::cout << "   | typecheck |  REQUIRED  |             |             |              " << std::endl;
    std::cout << "   +-----------+------------+-------------+-------------+              " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "Flags must be specified in such a way that following                   " << std::endl;
    std::cout << "constraints are satisfied:                                             " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "-i --input:                                                            " << std::endl;
    std::cout << "  Must be followed by a space-separated list of                        " << std::endl;
    std::cout << "  relative or absolute file-paths to source-code files.                " << std::endl;
    std::cout << "  Such input-files must end with either `.basalt` or `.bt`.            " << std::endl;
    std::cout << "  File extension must be included.                                     " << std::endl;
    std::cout << "  No duplicate files are allowed.                                      " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "-o --output:                                                           " << std::endl;
    std::cout << "  Must be followed by a space-separated list of  output files.         " << std::endl;
    std::cout << "  Such files must end with one of the following extensions:            " << std::endl;
    std::cout << "     `.ll` `.llvm` for an LLVM-IR output                               " << std::endl;
    std::cout << "     `.s`  `.asm`  for an assembly output                              " << std::endl;
    std::cout << "     `.o`  `.obj`  for a binary object file output                     " << std::endl;
    std::cout << "  Two files must not produce the same kind of output.                  " << std::endl;
    std::cout << "                                                                       " << std::endl;
    std::cout << "-t --target:                                                           " << std::endl;
    std::cout << "  Must be followed by a target-triple.                                 " << std::endl;
    std::cout << "  The triple has the general format <arch><sub>-<vendor>-<sys>-<env>.  " << std::endl;
}