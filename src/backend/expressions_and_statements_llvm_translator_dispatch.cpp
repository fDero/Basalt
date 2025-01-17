//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_statement_to_llvm(
    llvm::BasicBlock* block,
    const Statement& statement
) {
    switch (statement.statement_kind()) {
        case StatementBody::Kind::assignment: return translate_assignment_to_llvm(block, statement.get<Assignment>());
        case StatementBody::Kind::variable_declaration: return translate_variable_declaration_to_llvm(block, statement.get<VariableDeclaration>());
        case StatementBody::Kind::const_declaration: return translate_constant_declaration_to_llvm(block, statement.get<ConstDeclaration>());
        case StatementBody::Kind::function_call: return translate_void_function_call_to_llvm(block, statement.get<FunctionCall>());
        case StatementBody::Kind::return_statement: return translate_return_statement_to_llvm(block, statement.get<Return>());
        case StatementBody::Kind::break_statement: return translate_break_statement_to_llvm(block, statement.get<Break>());
        case StatementBody::Kind::continue_statement: return translate_continue_statement_to_llvm(block, statement.get<Continue>());
        case StatementBody::Kind::conditional: return translate_conditional_to_llvm(block, statement.get<Conditional>());
        case StatementBody::Kind::while_loop: return translate_while_loop_to_llvm(block, statement.get<WhileLoop>());
        case StatementBody::Kind::until_loop: return translate_until_loop_to_llvm(block, statement.get<UntilLoop>());
    }
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expression_to_llvm(
    llvm::BasicBlock* block, 
    const Expression& expression
) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::array_literal:          return translate_array_literal_to_llvm(block, expression.get<ArrayLiteral>());
        case ExpressionBody::Kind::binary_operator:        return translate_binary_operator_to_llvm(block, expression.get<BinaryOperator>());
        case ExpressionBody::Kind::bool_literal:           return translate_boolean_literal_to_llvm(block, expression.get<BoolLiteral>());
        case ExpressionBody::Kind::char_literal:           return translate_character_literal_to_llvm(block, expression.get<CharLiteral>());
        case ExpressionBody::Kind::float_literal:          return translate_floating_literal_to_llvm(block, expression.get<FloatLiteral>());
        case ExpressionBody::Kind::function_call:          return translate_expr_function_call_to_llvm(block, expression.get<FunctionCall>());
        case ExpressionBody::Kind::identifier:             return translate_identifier_to_llvm(block, expression.get<Identifier>());
        case ExpressionBody::Kind::int_literal:            return translate_integer_literal_to_llvm(block, expression.get<IntLiteral>());
        case ExpressionBody::Kind::string_literal:         return translate_string_literal_to_llvm(block, expression.get<StringLiteral>());
        case ExpressionBody::Kind::type_operator:          return translate_type_operator_to_llvm(block, expression.get<TypeOperator>());
        case ExpressionBody::Kind::unary_operator:         return translate_unary_operator_to_llvm(block, expression.get<UnaryOperator>());
        case ExpressionBody::Kind::dot_member_access:      return translate_dot_member_access_to_llvm(block, expression.get<DotMemberAccess>());
        case ExpressionBody::Kind::square_bracket_access:  return translate_square_bracket_access_to_llvm(block, expression.get<SquareBracketsAccess>());
    }
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_type_operator_to_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& expr
) {
    switch (expr.operator_text.front()) {
        case 'i': return translate_is_operator_to_llvm(block, expr);
        case 'a': return translate_as_operator_to_llvm(block, expr);
        default: throw_unrecognized_type_operator(expr);
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_unary_operator_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& expr
) {
    OperatorKind operator_kind = operator_kinds.at(expr.operator_text);
    switch (operator_kind) {
        case OperatorKind::boolean_not:            return translate_boolean_not_to_llvm(block, expr);
        case OperatorKind::math_increment:         return translate_increment_prefix_to_llvm(block, expr);
        case OperatorKind::math_decrement:         return translate_decrement_prefix_to_llvm(block, expr);
        case OperatorKind::minus_operator:         return translate_minus_sign_to_llvm(block, expr);
        case OperatorKind::plus_operator:          return translate_plus_sign_to_llvm(block, expr);
        case OperatorKind::pointer_dereference_op: return translate_ptr_dereference_to_llvm(block, expr);
        case OperatorKind::address_of:             return translate_addressof_to_llvm(block, expr);
        default: throw_unrecognized_unary_operator(expr);
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& expr
) {
    OperatorKind operator_kind = operator_kinds.at(expr.operator_text);
    switch (operator_kind) {
        case OperatorKind::plus_operator:         return translate_plus_binary_operator_to_llvm(block, expr); 
        case OperatorKind::minus_operator:        return translate_minus_binary_operator_to_llvm(block, expr);
        case OperatorKind::mul_operator:          return translate_mul_binary_operator_to_llvm(block, expr);  
        case OperatorKind::div_operator:          return translate_div_binary_operator_to_llvm(block, expr);  
        case OperatorKind::mod_operator:          return translate_mod_binary_operator_to_llvm(block, expr);  
        case OperatorKind::less_then:             return translate_LT_then_comparison_to_llvm(block, expr);   
        case OperatorKind::greater_then:          return translate_GT_comparison_to_llvm(block, expr);        
        case OperatorKind::less_then_or_equal:    return translate_LE_comparison_to_llvm(block, expr);        
        case OperatorKind::greater_then_or_equal: return translate_GE_comparison_to_llvm(block, expr);        
        case OperatorKind::equal:                 return translate_EQ_comparison_to_llvm(block, expr);        
        case OperatorKind::not_equal:             return translate_NE_comparison_to_llvm(block, expr);        
        case OperatorKind::and_operator:          return translate_boolean_and_to_llvm(block, expr);          
        case OperatorKind::or_operator:           return translate_boolean_or_to_llvm(block, expr);           
        case OperatorKind::xor_operator:          return translate_boolean_xor_to_llvm(block, expr);          
        default: throw_unrecognized_binary_operator(expr);
    }
}