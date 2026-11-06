//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "core/project_file_structure.hpp"
#include "core/program_representation.hpp"
#include "preprocessing/preprocessor.hpp"
#include "backend/code_generator.hpp"
#include "frontend/user_command.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/display_utilities.hpp"
#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "frontend/manual.hpp"

using Command = UserCommand::SubCommand;
using Extension = FileDescriptor::FileExtension;


int main(int argc, char** argv) {
    try {
        auto command = UserCommand(argc, argv);
        auto inputs  = command.parse_inputs();
        auto outputs = command.parse_outputs();
        auto links   = command.parse_links();
        auto target  = command.parse_target();

        if (command.subcommand == Command::version) {
            std::cout << "version: 0.0.0 pre-release" << std::endl;
            exit(EXIT_SUCCESS);
        }

        if (command.subcommand == Command::help) {
            std::cout << BANNER_MESSAGE << std::endl;
            exit(EXIT_SUCCESS);
        }

        auto project = ProjectFileStructure();
        for (const std::string& file_name : command.inputs) {
            auto file_descriptor = FileDescriptor(file_name);
            auto tokenizer = Tokenizer(file_descriptor);
            auto parser = Parser(file_descriptor, tokenizer.tokenize());
            project.store_file_representation(parser.parse_everything());
        }

        auto ast = ProgramRepresentation(project);
        auto preprocessor = PreProcessor(ast);
        preprocessor.preprocess_packages_typename_conflicts();
        preprocessor.preprocess_type_definitions();
        preprocessor.preprocess_function_definitions();

        auto gen = CodeGenerator(ast, target, links);
        auto module = gen.initialize_module();
        auto machine = gen.initialize_target();
        if (command.subcommand == Command::exec) {
            std::cout << "Executing code...\n";
            auto exit_code = gen.run(gen.initialize_module());
            std::cout << "Execution output: " << exit_code;
            exit(EXIT_SUCCESS);
        }

        if (command.subcommand != Command::build) {
            assert(command.subcommand == Command::check);
            exit(EXIT_SUCCESS);
        }

        for (const FileDescriptor& out : outputs) {
            switch (out.extension) {
                case Extension::assembly: gen.emit_assembly(*module, machine, out); continue;
                case Extension::object: gen.emit_object(*module, machine, out); continue;
                case Extension::llvm: gen.emit_llvm(*module, machine, out); continue;
                default: throw_unrecognized_file_extension(out);
            }
        }
    }
    catch(...) {
        std::exception_ptr error = std::current_exception();
        display_pretty_error_message(error);
        exit(EXIT_FAILURE);
    }
}
