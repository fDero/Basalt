
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

[[nodiscard]] TypeSignature UnaryOperator::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& scoped_data
) const {
    if (operator_text == "!"){
        ensure_not_operator_unique_operand_is_of_type_bool(*this, operand.get_type(program, scoped_data));
        return PrimitiveType{ "Bool" };
    }
    else if (operator_text == "-" || operator_text == "+" || operator_text == "--" || operator_text == "++"){
        const TypeSignature& operand_type = operand.get_type(program, scoped_data);
        ensure_mathematical_prefix_operator_unique_operand_is_numerical(*this, operand_type);
        return operand_type;
    }
    else if (operator_text == "#"){
        const TypeSignature& operand_type = operand.get_type(program, scoped_data);
        ensure_ptr_dereference_unique_operand_is_pointer(*this, operand_type);
        return operand_type.get<PointerType>().pointed_type;
    }
    else if (operator_text == "&"){
        const TypeSignature& operand_type = operand.get_type(program, scoped_data);
        return PointerType { operand_type };
    }
    else {
        throw_invalid_unary_operator_use(*this, operand.get_type(program, scoped_data));
    }
}

[[nodiscard]] TypeSignature FunctionCall::get_type(
    const ProgramRappresentation& program, 
    const ScopedData& scoped_data
) const {
    const std::vector<TypeSignature> argument_types = get_arguments_types(program, scoped_data);
    const FunctionDefinition& fdef = program.func_defs_register.retrieve(function_name, argument_types);
    ensure_function_is_non_void_in_expression_evaluation(*this, fdef);
    return fdef.return_type.value();
}

[[nodiscard]] std::vector<TypeSignature> FunctionCall::get_arguments_types(
    const ProgramRappresentation& program, 
    const ScopedData& scoped_data
) const {
    std::vector<TypeSignature> types;
    types.reserve(arguments.size());
    for (const Expression& arg : arguments){
        types.push_back(arg.get_type(program, scoped_data));
    }
    return types;
}