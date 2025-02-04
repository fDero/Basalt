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
    FunctionDefinitionsRegister& function_definitions_register,
    CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine,
    ProjectFileStructure& project_file_structure,
    ScopeContext& scope_context
)
    : type_definitions_register(program_representation)
    , function_definitions_register(function_definitions_register)
    , common_feature_adoption_plan_generation_engine(common_feature_adoption_plan_generation_engine)
    , project_file_structure(project_file_structure)
    , scope_context(scope_context)
{}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_expression_type(
    const Expression& expression
) {    
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::bool_literal:          return deduce_primtive_type(bool_type, expression);
        case ExpressionBody::Kind::int_literal:           return deduce_primtive_type(int_type, expression);
        case ExpressionBody::Kind::float_literal:         return deduce_primtive_type(float_type, expression);
        case ExpressionBody::Kind::char_literal:          return deduce_primtive_type(char_type, expression);
        case ExpressionBody::Kind::string_literal:        return deduce_primtive_type(string_type, expression);
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

TypeSignature ExpressionTypeDeducer::deduce_primtive_type(
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
        function_definitions_register, 
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
    switch (type_operator.operator_text[0]) {
        case 'i': return deduce_primtive_type(bool_type, expression);
        case 'a': return operand_type;
        default: assert_unreachable();
    }
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_binary_operator(const Expression& expression) {
    assert_expression_is<BinaryOperator>(expression);
    const BinaryOperator& binary_operator = expression.get<BinaryOperator>();
    switch (binary_operator.binary_op_kind) {
        case BinaryOperator::Kind::boolean_and:
        case BinaryOperator::Kind::boolean_or:
        case BinaryOperator::Kind::boolean_xor:
            return deduce_type_from_logical_binary_operator(binary_operator);
        
        case BinaryOperator::Kind::cmp_eq:
        case BinaryOperator::Kind::cmp_neq:
            return deduce_type_from_eq_binary_operator(binary_operator);

        case BinaryOperator::Kind::cmp_gt:
        case BinaryOperator::Kind::cmp_geq:
        case BinaryOperator::Kind::cmp_lt:
        case BinaryOperator::Kind::cmp_leq:
            return deduce_type_from_ord_binary_operator(binary_operator);
        
        case BinaryOperator::Kind::math_sum:
        case BinaryOperator::Kind::math_sub:
        case BinaryOperator::Kind::math_mul:
        case BinaryOperator::Kind::math_div:
        case BinaryOperator::Kind::math_mod:
            return deduce_type_from_math_binary_operator(binary_operator);
    }
    assert_unreachable();
}

std::optional<TypeSignature> 
ExpressionTypeDeducer::deduce_type_from_unary_operator(const Expression& expression) {
    assert_expression_is<UnaryOperator>(expression);
    const UnaryOperator& unary_operator = expression.get<UnaryOperator>();
    switch (unary_operator.unary_op_kind) {
        case UnaryOperator::Kind::address_of:          return deduce_address_operator_type(unary_operator);
        case UnaryOperator::Kind::pointer_dereference: return deduce_pointer_dereference_operator_type(unary_operator);
        case UnaryOperator::Kind::boolean_not:         return deduce_boolean_not_operator_type(unary_operator);
        case UnaryOperator::Kind::plus_sign:           return deduce_math_prefix_operator_type(unary_operator);
        case UnaryOperator::Kind::minus_sign:          return deduce_math_prefix_operator_type(unary_operator);
    }
    assert_unreachable();
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