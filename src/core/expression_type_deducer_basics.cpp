//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/expression_type_deducer.hpp"
#include "core/function_call_resolver.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

ExpressionTypeDeducer::ExpressionTypeDeducer(
    TypeDefinitionsRegister& program_representation,
    OverloadingResolutionEngine& overloading_resolution_engine,
    CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine,
    ProjectFileStructure& project_file_structure,
    ScopeContext& scope_context
)
    : type_definitions_register(program_representation)
    , overloading_resolution_engine(overloading_resolution_engine)
    , common_feature_adoption_plan_generation_engine(common_feature_adoption_plan_generation_engine)
    , project_file_structure(project_file_structure)
    , scope_context(scope_context)
{}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_expression_type(
    const Expression& expression
) {    
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
        case ExpressionBody::Kind::array_literal:         return deduce_type_from_array_literal(expression);
        case ExpressionBody::Kind::type_operator:         return deduce_type_from_type_operator(expression);
        case ExpressionBody::Kind::binary_operator:       return deduce_type_from_binary_operator(expression);
        case ExpressionBody::Kind::unary_operator:        return deduce_type_from_unary_operator(expression);
    }
    assert_unreachable();
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_primtive_type(
    const std::string& type_name, 
    const Expression& expression
) {
    return PrimitiveType(
        type_name, 
        expression.as_debug_informations_aware_entity()
    );
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_type_from_identifier(const Expression& expression) {
    const TypeSignature& type = scope_context.get_local_object_type(expression.get<Identifier>().name);
    return type_definitions_register.unalias_type(type);
}

std::vector<TypeSignature> 
ExpressionTypeDeducer::deduce_argument_types_from_function_call(const FunctionCall& function_call) {
    std::vector<TypeSignature> argument_types;
    for (const Expression& argument : function_call.arguments) {
        std::optional<TypeSignature> argument_type = deduce_expression_type(argument);
        if (argument_type.has_value()) {
            argument_types.push_back(argument_type.value());
        }
    }
    return argument_types;
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_type_from_function_call(const Expression& expression) {
    assert_expression_is<FunctionCall>(expression);
    const FunctionCall& function_call = expression.get<FunctionCall>();
    FunctionCallResolver resolver(
        type_definitions_register,
        overloading_resolution_engine, 
        common_feature_adoption_plan_generation_engine
    );
    auto argument_types = deduce_argument_types_from_function_call(function_call);
    auto return_type = resolver.resolve_function_call_return_type(function_call, argument_types);
    auto opt_return_type = return_type.is<TypeSignature>() 
        ? (std::optional<TypeSignature>) return_type.get<TypeSignature>() 
        : (std::optional<TypeSignature>) std::nullopt;
    ensure_function_has_a_return_type(function_call, opt_return_type);
    return type_definitions_register.unalias_type(opt_return_type.value());
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_type_from_type_operator(const Expression& expression) {
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

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_binary_operator(const Expression& expression) {
    assert_expression_is<BinaryOperator>(expression);
    const BinaryOperator& binary_operator = expression.get<BinaryOperator>();
    auto operator_kind_search_outcome = operator_kinds.find(binary_operator.operator_text);
    assert_operator_kind_was_found(operator_kind_search_outcome, operator_kinds);
    OperatorKind operator_kind = operator_kind_search_outcome->second;
    switch (operator_kind) {
        case OperatorKind::or_operator:               return deduce_type_from_logical_binary_operator(binary_operator);
        case OperatorKind::and_operator:              return deduce_type_from_logical_binary_operator(binary_operator);
        case OperatorKind::xor_operator:              return deduce_type_from_logical_binary_operator(binary_operator);
        case OperatorKind::equal:                     return deduce_type_from_eq_binary_operator(binary_operator);
        case OperatorKind::not_equal:                 return deduce_type_from_eq_binary_operator(binary_operator);
        case OperatorKind::less_then:                 return deduce_type_from_ord_binary_operator(binary_operator);
        case OperatorKind::greater_then:              return deduce_type_from_ord_binary_operator(binary_operator);
        case OperatorKind::less_then_or_equal:        return deduce_type_from_ord_binary_operator(binary_operator);
        case OperatorKind::greater_then_or_equal:     return deduce_type_from_ord_binary_operator(binary_operator);
        case OperatorKind::plus_operator:             return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::minus_operator:            return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::mul_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::div_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        case OperatorKind::mod_operator:              return deduce_type_from_math_binary_operator(binary_operator);
        default: assert_unreachable();
    }
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_type_from_unary_operator(const Expression& expression) {
    assert_expression_is<UnaryOperator>(expression);
    const UnaryOperator& unary_operator = expression.get<UnaryOperator>();
    auto operator_kind_search_outcome = operator_kinds.find(unary_operator.operator_text);
    ensure_operator_kind_was_found(operator_kind_search_outcome, operator_kinds);
    OperatorKind operator_kind = operator_kind_search_outcome->second;
    switch (operator_kind) {
        case OperatorKind::address_of:             return deduce_address_operator_type(unary_operator);
        case OperatorKind::pointer_dereference_op: return deduce_pointer_dereference_operator_type(unary_operator);
        case OperatorKind::boolean_not:            return deduce_boolean_not_operator_type(unary_operator);
        case OperatorKind::math_increment:         return deduce_math_prefix_operator_type(unary_operator);
        case OperatorKind::math_decrement:         return deduce_math_prefix_operator_type(unary_operator);
        case OperatorKind::plus_operator:          return deduce_math_prefix_operator_type(unary_operator);
        case OperatorKind::minus_operator:         return deduce_math_prefix_operator_type(unary_operator);
        default: assert_unreachable();
    }
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_type_from_array_literal(const Expression& expression) {
    assert_expression_is<ArrayLiteral>(expression);
    const ArrayLiteral& array_literal = expression.get<ArrayLiteral>();
    return ArrayType {
        expression.as_debug_informations_aware_entity(),
        array_literal.elements.size(),
        array_literal.stored_type
    };
}