
#pragma once
#include "toolchain/tokenizer.hpp"
#include "language/typesystem.hpp"
#include "language/statements.hpp"
#include "language/expressions.hpp"
#include "language/syntax.hpp"
#include <vector>
#include <string>
#include <iostream>

struct FunctionDefinition {
    
    std::string filename;
    unsigned long line_number;
    unsigned int tok_num;
    unsigned int char_pos;

    struct Argument {
        std::string arg_name;
        TypeSignature arg_type;
    };

    std::string pattern_tag_name;
    std::string function_id;

    std::string function_name;
    std::optional<TypeSignature> return_type;
    std::vector<std::string> template_generics_names;
    std::vector<Argument> arguments;
    std::vector<Statement> code;

    FunctionDefinition(const Token& func_token){
        filename = func_token.filename;
        line_number = func_token.line_number;
        tok_num = func_token.tok_number;
        char_pos = func_token.char_pos;
        function_name = func_token.sourcetext;
        function_id =  filename + "/" + std::to_string(line_number) + "/" +
            std::to_string(char_pos) + "/" + std::to_string(tok_num) + "/" + function_name;
    }

    void generate_pattern_tag_name(){

    }
};


struct StructDefinition {

    std::string filename;
    unsigned long line_number;
    unsigned int tok_num;
    unsigned int char_pos;

    struct Field {
        std::string field_name;
        TypeSignature field_type;
    };

    std::string home_package;
    std::string pattern_tag_name;
    std::string struct_id;

    std::string struct_name;
    std::vector<Field> fields;
    std::vector<std::string> template_generics_names;

    StructDefinition(const Token& struct_token){
        filename = struct_token.filename;
        line_number = struct_token.line_number;
        tok_num = struct_token.tok_number;
        char_pos = struct_token.char_pos;
        struct_name = struct_token.sourcetext;
        generate_struct_id();
        generate_pattern_tag_name();
    }

    void generate_struct_id(){
        struct_id = filename + "/" + std::to_string(line_number) + "/" +
            std::to_string(char_pos) + "/" + std::to_string(tok_num) + "/" + struct_name;
    }

    void generate_pattern_tag_name(){
        pattern_tag_name = struct_name;
        int number_of_generics = template_generics_names.size();
        if (number_of_generics > 0) {
            pattern_tag_name += "<";
            for (int i = 0; i < number_of_generics; i++) { 
                pattern_tag_name += "?,";
            }
            pattern_tag_name.back() = '>';
        }
    }

    void instanciate_generics(const BaseType& concrete_type);
};