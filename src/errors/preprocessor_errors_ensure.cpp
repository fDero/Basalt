
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "toolchain/rappresentation.hpp"
#include <unordered_set>

void ensure_type_not_already_visited_hence_no_cyclic_dependency(
    const std::string& target_tag_name, 
    const std::unordered_set<std::string>& visited_definitions
){
    auto search_outcome = std::find(visited_definitions.begin(), visited_definitions.end(), target_tag_name);
    if (search_outcome != visited_definitions.end()){
        throw std::runtime_error ("Cyclic dependency!!");
    }
}

void ensure_no_multiple_definition_of_the_same_function(
    const std::pair<std::unordered_map<std::string, FunctionDefinition>::iterator, bool>& 
        function_definition_insertion_outcome
){
    if (!(function_definition_insertion_outcome.second)){
        throw std::runtime_error {
            "Multiple definition of the same function in different files\n"
        };
    }
}

void ensure_no_multiple_definition_of_the_same_type(
    const std::pair<std::map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
){
    if (!(type_definition_insertion_outcome.second)){
        throw std::runtime_error {
            "Multiple definition of the same struct in different files\n"
        };
    }
}

void ensure_function_is_non_void_in_expression_evaluation(
    const FunctionCall& function_call, 
    const FunctionDefinition& fdef
){
    if (!fdef.return_type.has_value()){
        throw std::runtime_error {
            "Function " + function_call.function_name + " doesn't return a value hence it cannot be used in an expression\n"
        };
    }
}

[[noreturn]] void throw_invalid_unary_operator_use(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    throw std::runtime_error {
        "Invalid use of unary operator " + unary_operator.operator_text + " with operand of type " + operand_type.to_string() + "\n"
    };
}

[[noreturn]] void throw_invalid_binary_operator_use(
    const BinaryOperator& binary_operator, 
    const TypeSignature& left_operand_type, 
    const TypeSignature& right_operand_type
){
    throw std::runtime_error {
        "Invalid use of binary operator " + binary_operator.operator_text + " with operands of types " + left_operand_type.to_string() + " and " + right_operand_type.to_string() + "\n"
    };
}

void ensure_ptr_dereference_unique_operand_is_pointer(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<PointerType>()){
        throw std::runtime_error {
            "Invalid use of pointer dereference operator on non-pointer type " + operand_type.to_string() + "\n"
        };
    }
}

void ensure_mathematical_prefix_operator_unique_operand_is_numerical(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<BaseType>() || 
        (operand_type.get<BaseType>().type_name != "Int" && operand_type.get<BaseType>().type_name != "Float")){
            throw std::runtime_error {
                "Invalid use of pointer dereference operator on non-pointer type " + operand_type.to_string() + "\n"
            };
    }
}

void ensure_not_operator_unique_operand_is_of_type_bool(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<BaseType>() || operand_type.get<BaseType>().type_name != "Bool"){
        throw std::runtime_error {
            "Invalid use of pointer dereference operator on non-pointer type " + operand_type.to_string() + "\n"
        };
    }
}

void enure_left_operand_is_array_type_when_deducing_expression_type(
    const BinaryOperator& binary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<ArrayType>()){
        throw std::runtime_error {
            "invalid use of square brackets access operator with bad accessed type: " + operand_type.to_string() + "\n"
        };
    }
}

void enrue_right_operand_is_array_index_type_when_deducing_expression_type(
    const BinaryOperator& binary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<BaseType>() || operand_type.get<BaseType>().type_name != "Int"){
        throw std::runtime_error {
            "invalid use of square brackets access operator with bad index type: " + operand_type.to_string() + "\n"
        };
    }
}

void ensure_types_are_equality_comparable(
    const BinaryOperator& binary_operator, 
    const TypeSignature& left_type, 
    const TypeSignature& right_type
){
    if (left_type.to_string() != right_type.to_string()){
        throw std::runtime_error {
            "Invalid use of equality operator with non-equality-comparable types: " + left_type.to_string() + " and " + right_type.to_string() + "\n"
        };
    }
}

void ensure_type_not_already_found(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_result, 
    const std::pair<std::string, TypeDefinition>& match_pattern_cursor, 
    const FileRappresentation& file_rappresentation, 
    const std::string& package
){
    if (!insertion_result.second){
        throw std::runtime_error {
            "Type " + match_pattern_cursor.first + " already found in package " + package + " while visiting file " + file_rappresentation.file_metadata.filename + "\n"
        };
    }
}