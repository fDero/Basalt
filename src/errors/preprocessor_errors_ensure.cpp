
#include "errors/internal_errors.hpp"
#include "toolchain/rappresentation.hpp"
#include <unordered_set>

void ensure_struct_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name, 
    const std::unordered_set<std::string>& visited_structs
){
    auto search_outcome = std::find(visited_structs.begin(), visited_structs.end(), target_tag_name);
    if (search_outcome != visited_structs.end()){
        throw std::runtime_error ("Cyclic dependency!!");
    }
}

void ensure_no_multiple_definition_of_the_same_function(
    const std::pair<std::unordered_map<std::string, FunctionDefinition>::iterator, bool>& 
        function_definition_insertion_outcome
){
    if (!(function_definition_insertion_outcome.second)){
        throw ParsingError {
            "Multiple definition of the same function in different files\n", Token {}
        };
    }
}

void ensure_no_multiple_definition_of_the_same_struct(
    const std::pair<std::map<std::string, TypeDefinition>::iterator, bool>& 
        struct_definition_insertion_outcome
){
    if (!(struct_definition_insertion_outcome.second)){
        throw ParsingError {
            "Multiple definition of the same struct in different files\n", Token {}
        };
    }
}