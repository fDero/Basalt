
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