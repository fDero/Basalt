
#include "toolchain/tokenizer.hpp"
#include "language/definitions.hpp"
#include "errors/error-types.hpp"
#include "language/functions.hpp"
#include <unordered_map>
#include <unordered_set>

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
);

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome
);

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved
);

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
);

void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier, 
    const bool search_outcome
);

void ensure_parent_scope_exists_for_further_local_object_search(
    const ScopeContext* parent_scope,
    const std::string& identifier
);

void ensure_object_is_mutable(bool is_const);

inline void ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(
    const std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRuleSet::Ref>>& best_maches_so_far
){
    if (best_maches_so_far.size() > 1) {
        throw std::runtime_error("ambiguous function call");
    }
}

inline void assert_number_of_arguments_match(
    FunctionDefinition::Ref function_definition,
    const std::vector<TypeSignature>& arg_types
){
    if (function_definition->arguments.size() != arg_types.size()) {
        throw InternalError("number of arguments does not match");
    }
}