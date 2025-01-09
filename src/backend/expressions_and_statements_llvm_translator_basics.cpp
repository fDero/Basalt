//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;

ExpressionsAndStatementsLLVMTranslator::ExpressionsAndStatementsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator,
    TranslationAwareScopeContext scope_context,
    llvm::LLVMContext& context,
    llvm::IRBuilder<>& builder,
    llvm::Function* current_function,
    llvm::BasicBlock* entry_block,
    llvm::BasicBlock* exit_block
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
    , callable_codeblocks_llvm_translator(callable_codeblocks_llvm_translator)
    , scope_context(scope_context)
    , context(context)
    , builder(builder)
    , loop_entry_block(entry_block)
    , loop_exit_block(exit_block)
    , current_function(current_function)
{ }

[[nodiscard]] ExpressionsAndStatementsLLVMTranslator 
ExpressionsAndStatementsLLVMTranslator::create_translator_for_nested_loop(
    llvm::BasicBlock* new_entry_block,
    llvm::BasicBlock* new_exit_block
){
    return ExpressionsAndStatementsLLVMTranslator(
        program_representation, 
        type_definitions_llvm_translator, 
        callable_codeblocks_llvm_translator, 
        scope_context.create_nested_scope(), 
        context, 
        builder,  
        current_function,
        new_entry_block, 
        new_exit_block
    );
}

[[nodiscard]] ExpressionsAndStatementsLLVMTranslator 
ExpressionsAndStatementsLLVMTranslator::create_translator_for_nested_conditional(){
    return create_translator_for_nested_loop(loop_entry_block, loop_exit_block);
}

void ExpressionsAndStatementsLLVMTranslator::translate_statement_into_llvm(
    const Statement& statement
){
    switch (statement.statement_kind()) {
        break; case StatementBody::Kind::assignment: translate_assignment_into_llvm(statement.get<Assignment>());
        break; case StatementBody::Kind::variable_declaration: translate_variable_declaration_into_llvm(statement.get<VariableDeclaration>());
        break; case StatementBody::Kind::const_declaration: translate_constant_declaration_into_llvm(statement.get<ConstDeclaration>());
        break; case StatementBody::Kind::function_call: translate_void_function_call_into_llvm(statement.get<FunctionCall>());
        break; case StatementBody::Kind::return_statement: translate_return_statement_into_llvm(statement.get<Return>());
        break; case StatementBody::Kind::break_statement: translate_break_statement_into_llvm(statement.get<Break>());
        break; case StatementBody::Kind::continue_statement: translate_continue_statement_into_llvm(statement.get<Continue>());
        break; case StatementBody::Kind::conditional: translate_conditional_into_llvm(statement.get<Conditional>());
        break; case StatementBody::Kind::while_loop: translate_while_loop_into_llvm(statement.get<WhileLoop>());
        break; case StatementBody::Kind::until_loop: translate_until_loop_into_llvm(statement.get<UntilLoop>());
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expression_into_llvm(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::array_literal:          return translate_array_literal_into_llvm(expression.get<ArrayLiteral>());
        case ExpressionBody::Kind::binary_operator:        return translate_binary_operator_into_llvm(expression.get<BinaryOperator>());
        case ExpressionBody::Kind::bool_literal:           return translate_boolean_literal_into_llvm(expression.get<BoolLiteral>());
        case ExpressionBody::Kind::char_literal:           return translate_character_literal_into_llvm(expression.get<CharLiteral>());
        case ExpressionBody::Kind::float_literal:          return translate_floating_literal_into_llvm(expression.get<FloatLiteral>());
        case ExpressionBody::Kind::function_call:          return translate_expr_function_call_into_llvm(expression.get<FunctionCall>());
        case ExpressionBody::Kind::identifier:             return translate_identifier_into_llvm(expression.get<Identifier>());
        case ExpressionBody::Kind::int_literal:            return translate_integer_literal_into_llvm(expression.get<IntLiteral>());
        case ExpressionBody::Kind::string_literal:         return translate_string_literal_into_llvm(expression.get<StringLiteral>());
        case ExpressionBody::Kind::type_operator:          return translate_type_operator_into_llvm(expression.get<TypeOperator>());
        case ExpressionBody::Kind::unary_operator:         return translate_unary_operator_into_llvm(expression.get<UnaryOperator>());
        //case ExpressionBody::Kind::dot_member_access:      return translate_dot_member_access_into_llvm(expression.get<DotMemberAccess>());
        //case ExpressionBody::Kind::square_bracket_access:  return translate_square_bracket_access_into_llvm(expression.get<SquareBracketAccess>());
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expr_function_call_into_llvm(const FunctionCall& expr) {
    CallableCodeBlock ccb = program_representation.resolve_function_call(expr, scope_context.raw_scope_context);
    llvm::Function* llvm_function = callable_codeblocks_llvm_translator.translate_callable_code_block_into_llvm(ccb);
    return builder.CreateCall(llvm_function);
}

void ExpressionsAndStatementsLLVMTranslator::translate_void_function_call_into_llvm(const FunctionCall& function_call) {
    std::ignore = translate_expr_function_call_into_llvm(function_call);
}