
#include "errors/parsing_errors.hpp"

void ensure_parenthesis_gets_closed_in_expression_wrapping(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const Token& parenthesis_opening_token

){
    if (iterator == source_tokens.end() || iterator->sourcetext != ")"){
        throw ParsingError {
            "parenthesis got opened but never closed",
            parenthesis_opening_token
        };
    }
}

void ensure_square_bracket_gets_closed_in_expression_wrapping(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const Token& brackets_opening_token

){
    if (iterator == source_tokens.end() || iterator->sourcetext != "]"){
        throw ParsingError {
            "square brackets got opened but never closed",
            brackets_opening_token
        };
    }
}

void ensure_struct_doesnt_already_exists(
    const std::string& struct_tag_name,
    const StructDefinition& struct_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
){
    if (type_definitions_register.find(struct_tag_name) != type_definitions_register.end()){
        throw ParsingError {
            "such user defined type already exists in the given scope",
            Token {}
        };
    }
}

void ensure_union_doesnt_already_exists(
    const std::string& union_tag_name,
    const UnionDefinition& union_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
){
    if (type_definitions_register.find(union_tag_name) != type_definitions_register.end()){
        throw ParsingError {
            "such user defined type already exists in the given scope",
            Token {}
        };
    }
}

void ensure_there_is_still_a_scope_in_wich_keep_searching_struct_definitions(
    const std::map<std::string, std::string>::const_iterator& parent_scope_search_outcome,
    const std::map<std::string, std::string>& struct_parents
){
        if (parent_scope_search_outcome == struct_parents.end()){
            throw ParsingError {
                "struct doesnt exists in this file or in the files imported by it",
                Token {}
            };
    }
}

void ensure_function_doesnt_already_exists(
    const std::string& func_tag_name,
    const FunctionDefinition& func_def,
    const std::unordered_map<std::string, FunctionDefinition>& function_definitions_register
){
    if (function_definitions_register.find(func_tag_name) != function_definitions_register.end()){
        throw ParsingError {
            "such function already exists in the given scope",
            Token {}
        };
    }
}

void ensure_overload_set_does_exist(
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator overload_set, 
    const std::string& func_name, const std::unordered_map<std::string, std::vector<std::string>> func_names_register
){
    if (overload_set == func_names_register.end()){
        throw ParsingError {
            "overload set doesn't exist",
            Token {}
        };
    }
}

void ensure_a_valid_function_overload_was_actually_found(const std::string& target_func_tag_name){
    if (target_func_tag_name.empty()){
        throw ParsingError {
            "overload set exists but no function in that overload set matches the function call",
            Token {}
        };
    }
}

void ensure_no_function_overload_was_found_before_this(
    const std::string& previous_func_tag_name, const std::string& current_func_tag_name,
    const std::unordered_map<std::string, FunctionDefinition>& function_definitions_register
){
    if (!previous_func_tag_name.empty()){
        throw ParsingError {
            "two ambiguous function definitions to choose from in the same overload set",
            Token {}
        };
    }
}