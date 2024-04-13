
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "errors/internal_errors.hpp"
#include <regex>

void FunctionDefinitionsRegister::store(const FunctionDefinition& func_def){
    std::string func_tag_name = func_def.function_name + " ";
    for (const FunctionDefinition::Argument& arg : func_def.arguments){
        func_tag_name += arg.arg_type.function_retrieval_match_string(
            func_def.template_generics_names) + " ";
    }
    func_tag_name.pop_back();
    ensure_function_doesnt_already_exists(func_tag_name, func_def, function_definitions_register);
    function_definitions_register.insert(std::make_pair(func_tag_name, func_def));
    function_names_register[func_def.function_name].push_back(func_tag_name);
}

[[nodiscard]] const FunctionDefinition& FunctionDefinitionsRegister::retrieve(
    const std::string& func_name, 
    const std::vector<TypeSignature>& argtypes
) const {
    std::string function_retrieve_string = craft_function_retrieve_string(func_name, argtypes);
    auto attempt_direct_retrieve = function_definitions_register.find(function_retrieve_string);
    if (attempt_direct_retrieve != function_definitions_register.end()){
        return attempt_direct_retrieve->second;
    }
    auto overload_set_search = function_names_register.find(func_name);
    ensure_overload_set_does_exist(overload_set_search, func_name, function_names_register);
    std::string target_func_tag_name = "";
    for (const std::string& func_tag_name : overload_set_search->second){
        if (std::regex_match(function_retrieve_string, std::regex(func_tag_name))){
            ensure_no_function_overload_was_found_before_this(
                target_func_tag_name, func_tag_name, function_definitions_register);
            target_func_tag_name = func_tag_name;
        }
    }
    ensure_a_valid_function_overload_was_actually_found(target_func_tag_name);
    auto finally_retrieved = function_definitions_register.find(target_func_tag_name);
    assert_function_actually_retrieved(finally_retrieved, function_definitions_register);
    return finally_retrieved->second;
}

[[nodiscard]] std::string FunctionDefinitionsRegister::craft_function_retrieve_string(
    const std::string& func_name, 
    const std::vector<TypeSignature>& argtypes
) {    
    std::string func_tag_name = func_name + " ";
    for (const TypeSignature& argtype : argtypes){
        func_tag_name += argtype.to_string() + " ";
    }
    func_tag_name.pop_back();
    return func_tag_name;
}