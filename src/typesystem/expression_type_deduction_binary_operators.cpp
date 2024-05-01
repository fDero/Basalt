
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

[[nodiscard]] TypeSignature BinaryOperator::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& scoped_data
) const {
    const TypeSignature& left_type = left_operand.get_type(program, scoped_data);
    const TypeSignature& right_type = right_operand.get_type(program, scoped_data);
    if (this->is_string_operator() && has_string_operands(left_type, right_type)) {
        return PrimitiveType{ "String" };
    }
    else if (this->is_numerical_operator() && this->has_int_operands(left_type, right_type)) {
        return PrimitiveType{ "Int" };
    }
    else if (this->is_numerical_operator() && this->has_numerical_operands(left_type, right_type)) {
        return PrimitiveType{ "Float" };
    }
    else if (this->is_comparison_operator() && this->is_valid_comparison(*this, program, left_type, right_type)) {
        return PrimitiveType{ "Bool" };
    }
    else if (this->is_logical_operator() && this->has_boolean_operands(left_type, right_type)) {
        return PrimitiveType{ "Bool" };
    }
    else if (this->operator_text == square_brackets_access){
        enure_left_operand_is_array_type_when_deducing_expression_type(*this, left_type);
        enrue_right_operand_is_array_index_type_when_deducing_expression_type(*this, right_type);
        return left_type.get<ArrayType>().stored_type;
    }
    else {
        throw_invalid_binary_operator_use(*this, left_type, right_type);
        throw;
    }
}

[[nodiscard]] bool BinaryOperator::is_numerical_operator() const {
    return operator_text == math_add_operator || operator_text == math_sub_operator 
        || operator_text == math_mul_operator || operator_text == math_div_operator
        || operator_text == math_mod_operator || operator_text == math_pow_operator;
}

[[nodiscard]] bool BinaryOperator::is_comparison_operator() const {
    return operator_text == boolean_less_than_operator 
        || operator_text == boolean_less_than_or_equal_operator
        || operator_text == boolean_greater_than_operator
        || operator_text == boolean_greater_than_or_equal_operator
        || operator_text == boolean_equal_operator
        || operator_text == boolean_not_equal_operator;
}

[[nodiscard]] bool BinaryOperator::is_logical_operator() const {
    return operator_text == boolean_and_operator 
        || operator_text == boolean_or_operator 
        || operator_text == boolean_xor_operator;
}

[[nodiscard]] bool BinaryOperator::is_string_operator() const {
    return operator_text == string_soft_concatenation_operator
        || operator_text == string_hard_concatenation_operator;
}

[[nodiscard]] bool BinaryOperator::has_int_operands(const TypeSignature& left, const TypeSignature& right) const {
    if (!left.is<PrimitiveType>()  || left.get<PrimitiveType>().type_name != "Int")  return false;
    if (!right.is<PrimitiveType>() || right.get<PrimitiveType>().type_name != "Int") return false;
    return true;
}

[[nodiscard]] bool BinaryOperator::has_boolean_operands(const TypeSignature& left, const TypeSignature& right) const {
    if (!left.is<PrimitiveType>()  || left.get<PrimitiveType>().type_name != "Bool")  return false;
    if (!right.is<PrimitiveType>() || right.get<PrimitiveType>().type_name != "Bool") return false;
    return true;
}

[[nodiscard]] bool BinaryOperator::has_numerical_operands(const TypeSignature& left, const TypeSignature& right) const {
    if (!left.is<PrimitiveType>()  || left.get<PrimitiveType>().type_name != "Int")  {
        if (!left.is<PrimitiveType>()  || left.get<PrimitiveType>().type_name != "Float") return false;
    }
    if (!right.is<PrimitiveType>() || right.get<PrimitiveType>().type_name != "Int") {
        if (!right.is<PrimitiveType>() || right.get<PrimitiveType>().type_name != "Float") return false;
    }
    return true;
}

[[nodiscard]] bool BinaryOperator::has_string_operands(const TypeSignature& left, const TypeSignature& right) const {
    if (!left.is<PrimitiveType>()  || left.get<PrimitiveType>().type_name != "String")  return false;
    if (!right.is<PrimitiveType>() || right.get<PrimitiveType>().type_name != "String") return false;
    return true;
}

[[nodiscard]] bool BinaryOperator::is_valid_comparison(
    const BinaryOperator& op, const ProgramRappresentation& program,
    const TypeSignature& left, const TypeSignature& right
) const {
    // ensure_binary_operator_operands_are_of_compatible_type(*this, left, right, program);
    return !(right.is<PrimitiveType>() && left.get<PrimitiveType>().type_name == "Bool");
}
