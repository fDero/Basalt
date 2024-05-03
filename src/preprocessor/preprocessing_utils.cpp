
#include "toolchain/preprocessor.hpp"
#include "toolchain/tokenizer.hpp"
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/display_utilities.hpp"
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <iostream>

PreProcessor::PreProcessor(const std::vector<std::string>& input_files) 
    : input_files(input_files) { }

void PreProcessor::parse_every_file() {
    for (const std::string& input_file : input_files){
        try {
            auto tokenizer = Tokenizer(std::filesystem::path(input_file));
            std::vector<Token> tokens = tokenizer.tokenize();
            auto parser = Parser(tokens);
            FileRappresentation parsed_output = parser.parse_everything();
            parsed_files_outcome.push_back(parsed_output);
        }
        catch (...){
            std::exception_ptr exception_capture;
            exception_capture = std::current_exception();
            errors_occurred_while_preprocessing.push_back(exception_capture);
        }
    }
    this->inspect_for_errors();
    this->collect_parsed_outputs();
}

void PreProcessor::inspect_for_errors(){
    for (std::exception_ptr& error : errors_occurred_while_preprocessing){
        display_pretty_error_message(error);
    }
    if (!errors_occurred_while_preprocessing.empty()){
        exit(EXIT_FAILURE);
    }
}

void PreProcessor::collect_parsed_outputs(){
    while (!parsed_files_outcome.empty()){
        const FileRappresentation& parsed_file = parsed_files_outcome.back();
        for (const TypeDefinition& type_definition : parsed_file.type_defs){
            types_register.store(type_definition);
        }
        for (const FunctionDefinition& func_definition : parsed_file.func_defs){
            funcs_register.store(func_definition);
        }
        parsed_files_outcome.pop_back();
    }
}

void PreProcessor::dump_internal_state(){
    std::cerr << "DUMPING\n";
    for (const auto& struct_data : types_register.struct_definitions){
        std::cerr << struct_data.first << " <- S tag name \n";
    }
    for (const auto& func_def_data : funcs_register.function_definitions_register){
        std::cerr << func_def_data.first << " <- F tag name \n";
    }
    for (const auto& func_name_data : funcs_register.function_names_register){
        std::cerr << func_name_data.first << " <- F overload set \n";
        for (const std::string& ovload : func_name_data.second){
            std::cerr << "[ " << ovload << " ]\n";    
        }
    }
    std::cerr << "DUMPED\n";
}