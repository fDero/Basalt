
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