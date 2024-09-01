
#include "typesystem/expression_type_deducer.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

ExpressionTypeDeducer::ExpressionTypeDeducer(
    TypeDefinitionsRegister& program_representation,
    OverloadingResolutionEngine& overloading_resolution_engine,
    ProjectFileStructure& project_file_structure,
    ScopeContext& scope_context
)
    : type_definitions_register(program_representation)
    , overloading_resolution_engine(overloading_resolution_engine)
    , project_file_structure(project_file_structure)
    , scope_context(scope_context)
{}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_expression_type(const Expression& expression) {    
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::bool_literal:          return deduce_primtive_type("Bool", expression);
        case ExpressionBody::Kind::int_literal:           return deduce_primtive_type("Int", expression);
        case ExpressionBody::Kind::float_literal:         return deduce_primtive_type("Float", expression);
        case ExpressionBody::Kind::char_literal:          return deduce_primtive_type("Char", expression);
        case ExpressionBody::Kind::string_literal:        return deduce_primtive_type("String", expression);
        case ExpressionBody::Kind::dot_member_access:     return deduce_type_from_dot_member_access(expression);
        case ExpressionBody::Kind::square_bracket_access: return deduce_type_from_square_brackets_access(expression);
        case ExpressionBody::Kind::identifier:            return deduce_type_from_identifier(expression);
        case ExpressionBody::Kind::function_call:         return deduce_type_from_function_call(expression);
        case ExpressionBody::Kind::array_literal:         return expression.get<ArrayLiteral>().stored_type;
        case ExpressionBody::Kind::type_operator:         return deduce_type_from_type_operator(expression);
        case ExpressionBody::Kind::binary_operator:       return deduce_type_from_binary_operator(expression);
        case ExpressionBody::Kind::unary_operator:        return deduce_type_from_unary_operator(expression);
    }
    assert_unreachable();
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_primtive_type(const std::string& type_name, const Expression& expression) {
    return PrimitiveType(
        type_name, 
        expression.as_debug_informations_aware_entity()
    );
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_identifier(const Expression& expression) {
    const TypeSignature& type = scope_context.get_local_object_type(expression.get<Identifier>().name);
    return type_definitions_register.unalias_type(type);
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_function_call(const Expression& expression) {
    assert_expression_is<FunctionCall>(expression);
    const FunctionCall& function_call = expression.get<FunctionCall>();
    const std::vector<Expression>& arguments = function_call.arguments;
    std::vector<TypeSignature> argument_types;
    for (const Expression& argument : arguments) {
        argument_types.push_back(deduce_expression_type(argument));
    }
    FunctionDefinition::Ref retrieved = overloading_resolution_engine.retrieve_function_definition(function_call, argument_types);
    ensure_function_overload_was_successfully_retrieved(function_call, retrieved);
    ensure_function_has_a_return_type(retrieved);
    const TypeSignature& type =  *retrieved->return_type;
    return type_definitions_register.unalias_type(type);
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_type_operator(const Expression& expression) {
    assert_expression_is<TypeOperator>(expression);
    const TypeOperator& type_operator = expression.get<TypeOperator>();
    const TypeSignature& operand_type = type_operator.typesignature;
    const auto& operator_kind_search_outcome = operator_kinds.find(type_operator.operator_text);
    assert_operator_kind_was_found(operator_kind_search_outcome, operator_kinds);
    const OperatorKind& operator_kind = operator_kind_search_outcome->second;
    switch (operator_kind) {
        case OperatorKind::is_operator: return deduce_primtive_type("Bool", expression);
        case OperatorKind::as_operator: return operand_type;
        default: assert_unreachable();
    }
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_binary_operator(const Expression& expression) {
    assert_expression_is<BinaryOperator>(expression);
    const BinaryOperator& binary_operator = expression.get<BinaryOperator>();
    auto operator_kind_search_outcome = operator_kinds.find(binary_operator.operator_text);
    assert_operator_kind_was_found(operator_kind_search_outcome, operator_kinds);
    OperatorKind operator_kind = operator_kind_search_outcome->second;
    switch (operator_kind) {
        case OperatorKind::or_operator:               return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::and_operator:              return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::xor_operator:              return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::equal:                     return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::not_equal:                 return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::less_then:                 return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::greater_then:              return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::less_then_or_equal:        return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::greater_then_or_equal:     return deduce_type_from_comparison_operator(binary_operator);
        case OperatorKind::plus_operator:             return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::minus_operator:            return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::mul_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::div_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::mod_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::pow_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        default: assert_unreachable();
    }
}

[[nodiscard]] TypeSignature ExpressionTypeDeducer::deduce_type_from_unary_operator(const Expression& expression) {
    assert_expression_is<UnaryOperator>(expression);
    const UnaryOperator& unary_operator = expression.get<UnaryOperator>();
    auto operator_kind_search_outcome = operator_kinds.find(unary_operator.operator_text);
    ensure_operator_kind_was_found(operator_kind_search_outcome, operator_kinds);
    OperatorKind operator_kind = operator_kind_search_outcome->second;
    switch (operator_kind) {
        case OperatorKind::address_op:             return deduce_address_operator_type(unary_operator);
        case OperatorKind::pointer_dereference_op: return deduce_pointer_dereference_operator_type(unary_operator);
        case OperatorKind::boolean_not:            return deduce_boolean_not_operator_type(unary_operator);
        case OperatorKind::math_increment:         return deduce_math_prefix_operator_type(unary_operator);
        case OperatorKind::math_decrement:         return deduce_math_prefix_operator_type(unary_operator);
        case OperatorKind::plus_operator:          return deduce_math_prefix_operator_type(unary_operator);
        case OperatorKind::minus_operator:         return deduce_math_prefix_operator_type(unary_operator);
        default: assert_unreachable();
    }
}