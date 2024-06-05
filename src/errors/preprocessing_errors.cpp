
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "toolchain/representation.hpp"
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
    const std::pair<std::unordered_map<std::string, TypeDefinition>::iterator, bool>& 
        type_definition_insertion_outcome
){
    if (!(type_definition_insertion_outcome.second)){
        throw std::runtime_error {
            "Multiple definition of the same type in different files\n"
        };
    }
}

void ensure_no_type_definition_conflict_detected(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_outcome
){
    if (!(insertion_outcome.second)){
        throw std::runtime_error {
            "Multiple definition of the same type in different packages, both included by the same file\n"
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
        "Invalid use of unary operator " + unary_operator.operator_text + " with operand of type " + "<operand_type>" + "\n"
    };
}

[[noreturn]] void throw_no_type_definition_found(
    const CustomType& type_signature
){
    throw std::runtime_error {
        "No type definition found for type " + type_signature.type_name + "\n"
    };
}

[[noreturn]] void throw_invalid_binary_operator_use(
    const BinaryOperator& binary_operator, 
    const TypeSignature& left_operand_type, 
    const TypeSignature& right_operand_type
){
    throw std::runtime_error {
        "Invalid use of binary operator " + binary_operator.operator_text + " with operands of types " + "<operand_type>" + " and " + "<operand_type>" + "\n"
    };
}

void ensure_ptr_dereference_unique_operand_is_pointer(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<PointerType>()){
        throw std::runtime_error {
            "Invalid use of pointer dereference operator on non-pointer type <operand_type> \n"
        };
    }
}

void ensure_mathematical_prefix_operator_unique_operand_is_numerical(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<CustomType>() || 
        (operand_type.get<CustomType>().type_name != "Int" && operand_type.get<CustomType>().type_name != "Float")){
            throw std::runtime_error {
                "Invalid use of pointer dereference operator on non-pointer type <operand_type> \n"
            };
    }
}

void ensure_not_operator_unique_operand_is_of_type_bool(
    const UnaryOperator& unary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<CustomType>() || operand_type.get<CustomType>().type_name != "Bool"){
        throw std::runtime_error {
            "Invalid use of pointer dereference operator on non-pointer type <operand_type> \n"
        };
    }
}

void enure_left_operand_is_array_type_when_deducing_expression_type(
    const BinaryOperator& binary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<ArrayType>()){
        throw std::runtime_error {
            "invalid use of square brackets access operator with bad accessed type: <operand_type> \n"
        };
    }
}

void enrue_right_operand_is_array_index_type_when_deducing_expression_type(
    const BinaryOperator& binary_operator, 
    const TypeSignature& operand_type
){
    if (!operand_type.is<CustomType>() || operand_type.get<CustomType>().type_name != "Int"){
        throw std::runtime_error {
            "invalid use of square brackets access operator with bad index type: <operand_type> \n"
        };
    }
}

void ensure_type_not_already_found(
    const std::pair<std::unordered_set<std::string>::iterator, bool>& insertion_result, 
    const std::pair<std::string, TypeDefinition>& match_pattern_cursor, 
    const Filerepresentation& file_representation, 
    const std::string& package
){
    if (!insertion_result.second){
        throw std::runtime_error {
            "Type " + match_pattern_cursor.first + " already found in package " + package + " while visiting file " + file_representation.file_metadata.filename + "\n"
        };
    }
}

void ensure_type_was_successfully_retrieved(
    const std::optional<std::string>& retrieved
){
    if (!retrieved.has_value()){
        throw std::runtime_error {
            "No type definition found\n"
        };
    }
}

void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier, 
    const bool search_outcome
){
    if (search_outcome){
        throw std::runtime_error {
            "Identifier " + identifier + " already exists in the current scope\n"
        };
    }
}

void ensure_parent_scope_exists_for_further_local_object_search(
    const ScopeContext* parent_scope, 
    const std::string& identifier
){
    if (parent_scope == nullptr){
        throw std::runtime_error {
            "Cannot find object: " + identifier + "\n"
        };
    }
}

void ensure_object_is_mutable(
    const bool is_const
){
    if (is_const){
        throw std::runtime_error {
            "Cannot modify a constant object\n"
        };
    }
}

[[noreturn]] void throw_no_function_definition_found(
    const PrecompiledFunctionCall& function_call
){
    throw std::runtime_error("no function defintion found!");
}