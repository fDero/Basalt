
#pragma once
#include "toolchain/tokenizer.hpp"
#include "toolchain/parser.hpp"
#include "language/syntax.hpp"
#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include <unordered_set>

struct StructDependencyNavigator {

    StructDependencyNavigator(StructDefinitionsRegister& structs_register);

    void visit_struct_definition(const StructDefinition& struct_definition);
    void visit_struct_field(const StructDefinition::Field& field, const std::vector<std::string>& struct_def_generics);
    void verify_that_the_type_exists(const TypeSignature& type_signature);

    StructDefinitionsRegister& structs_register;
    std::unordered_set<std::string> visited_structs;
};

class PreProcessor {

    public:
        PreProcessor(const std::vector<std::string>& input_files);

        void inspect_for_errors();
        void collect_parsed_outputs();
        void parse_every_file();
        void dump_internal_state();
        void preprocess_structs();

    private:
        std::vector<std::string> input_files;
        std::vector<FileRappresentation> parsed_files_outcome;
        std::vector<std::exception_ptr> errors_occurred_while_preprocessing;
        StructDefinitionsRegister structs_register;
        FunctionDefinitionsRegister funcs_register;
};