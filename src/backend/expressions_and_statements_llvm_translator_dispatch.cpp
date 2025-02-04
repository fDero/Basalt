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
    switch (expr.unary_op_kind) {
        case UnaryOperator::Kind::boolean_not:         return translate_boolean_not_to_llvm(block, expr);
        case UnaryOperator::Kind::minus_sign:          return translate_minus_sign_to_llvm(block, expr);
        case UnaryOperator::Kind::plus_sign:           return translate_plus_sign_to_llvm(block, expr);
        case UnaryOperator::Kind::pointer_dereference: return translate_ptr_dereference_to_llvm(block, expr);
        case UnaryOperator::Kind::address_of:          return translate_addressof_to_llvm(block, expr);
    }
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& expr
) {
    switch (expr.binary_op_kind) {
        case BinaryOperator::Kind::math_sum:     return translate_plus_binary_operator_to_llvm(block, expr); 
        case BinaryOperator::Kind::math_sub:     return translate_minus_binary_operator_to_llvm(block, expr);
        case BinaryOperator::Kind::math_mul:     return translate_mul_binary_operator_to_llvm(block, expr);  
        case BinaryOperator::Kind::math_div:     return translate_div_binary_operator_to_llvm(block, expr);  
        case BinaryOperator::Kind::math_mod:     return translate_mod_binary_operator_to_llvm(block, expr);  
        case BinaryOperator::Kind::cmp_lt:       return translate_LT_then_comparison_to_llvm(block, expr);   
        case BinaryOperator::Kind::cmp_gt:       return translate_GT_comparison_to_llvm(block, expr);        
        case BinaryOperator::Kind::cmp_leq:      return translate_LE_comparison_to_llvm(block, expr);        
        case BinaryOperator::Kind::cmp_geq:      return translate_GE_comparison_to_llvm(block, expr);        
        case BinaryOperator::Kind::cmp_eq:       return translate_EQ_comparison_to_llvm(block, expr);        
        case BinaryOperator::Kind::cmp_neq:      return translate_NE_comparison_to_llvm(block, expr);        
        case BinaryOperator::Kind::boolean_and:  return translate_boolean_and_to_llvm(block, expr);          
        case BinaryOperator::Kind::boolean_or:   return translate_boolean_or_to_llvm(block, expr);           
        case BinaryOperator::Kind::boolean_xor:  return translate_boolean_xor_to_llvm(block, expr);          
    }
    assert_unreachable();
}