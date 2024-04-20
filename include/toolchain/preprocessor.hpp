
#pragma once
#include "toolchain/tokenizer.hpp"
#include "toolchain/parser.hpp"
#include "language/syntax.hpp"
#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include <unordered_set>

struct TypeDependencyNavigator {

    TypeDependencyNavigator(TypeDefinitionsRegister& types_register);

    void visit_struct_definition(const StructDefinition& struct_definition);
    void visit_struct_field(const StructDefinition::Field& field, const std::vector<std::string>& struct_def_generics);
    void visit_union_definition(const UnionDefinition& union_definition);
    void visit_union_alternative(const TypeSignature& field, const std::vector<std::string>& union_def_generics);
    void verify_that_the_type_exists(const TypeSignature& type_signature);
    void instanciate_and_visit_union(const UnionDefinition& union_definition, const TypeSignature& concrete_type);
    void instanciate_and_visit_struct(const StructDefinition& struct_definition, const TypeSignature& concrete_type);

    TypeDefinitionsRegister& types_register;
    std::unordered_set<std::string> visited_definitions;
};

class PreProcessor {

    public:
        PreProcessor(const std::vector<std::string>& input_files);

        void inspect_for_errors();
        void collect_parsed_outputs();
        void parse_every_file();
        void dump_internal_state();
        void preprocess_type_defintions();

    private:
        std::vector<std::string> input_files;
        std::vector<FileRappresentation> parsed_files_outcome;
        std::vector<std::exception_ptr> errors_occurred_while_preprocessing;
        TypeDefinitionsRegister types_register;
        FunctionDefinitionsRegister funcs_register;
};