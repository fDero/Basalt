
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

inline void ensure_no_ambiguous_function_definition_found(
    const std::list<std::shared_ptr<FunctionDefinition>>& compatible_defs,
    const PrecompiledFunctionCall& precompiled_function_call
) {
    if (compatible_defs.size() > 1) {
        throw std::runtime_error {
            "Ambiguous function definition found for function call " + precompiled_function_call.original_function_call.function_name + "\n"
        };
    }
}

[[noreturn]] void throw_no_function_definition_found(
    const PrecompiledFunctionCall& function_call
);