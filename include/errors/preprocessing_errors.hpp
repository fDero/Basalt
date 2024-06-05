
#include "toolchain/tokenizer.hpp"
#include "language/definitions.hpp"
#include "errors/error-types.hpp"
#include "language/functions.hpp"
#include <unordered_map>
#include <unordered_set>

void ensure_function_is_non_void_in_expression_evaluation(
    const FunctionCall& function_call, 
    const FunctionDefinition& fdef
);

[[noreturn]] void throw_invalid_unary_operator_use(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
);

[[noreturn]] void throw_invalid_binary_operator_use(
    const BinaryOperator& binary_operator, 
    const TypeSignature& left_operand_type, 
    const TypeSignature& right_operand_type
);

void ensure_ptr_dereference_unique_operand_is_pointer(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
);

void ensure_mathematical_prefix_operator_unique_operand_is_numerical(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
);

void ensure_not_operator_unique_operand_is_of_type_bool(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
);

void enure_left_operand_is_array_type_when_deducing_expression_type(
    const BinaryOperator& binary_operator, 
    const TypeSignature& operand_type
);

void enrue_right_operand_is_array_index_type_when_deducing_expression_type(
    const BinaryOperator& binary_operator, 
    const TypeSignature& operand_type
);

void ensure_types_are_equality_comparable(
    const BinaryOperator& binary_operator, 
    const TypeSignature& left_type, 
    const TypeSignature& right_type
);

void ensure_type_not_already_found(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_result, 
    const std::pair<std::string, TypeDefinition>& match_pattern_cursor, 
    const Filerepresentation& file_representation, 
    const std::string& package
);

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
);

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome
);

void ensure_struct_doesnt_already_exists(
    const std::string& struct_tag_name,
    const StructDefinition& struct_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
);

void ensure_union_doesnt_already_exists(
    const std::string& union_tag_name,
    const UnionDefinition& union_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
);

void ensure_alias_doesnt_already_exists(
    const std::string& alias_tag_name,
    const TypeAlias& alias_def,
    const std::map<std::string, TypeDefinition>& type_definitions_register
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

void ensure_object_is_mutable(
    const bool is_const
);

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