//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;

ExpressionsAndStatementsLLVMTranslator::ExpressionsAndStatementsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator,
    TranslationAwareScopeContext scope_context,
    llvm::LLVMContext& context,
    llvm::Function* current_function,
    llvm::BasicBlock* entry_block,
    llvm::BasicBlock* exit_block
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
    , callable_codeblocks_llvm_translator(callable_codeblocks_llvm_translator)
    , scope_context(scope_context)
    , context(context)
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
        current_function,
        new_entry_block, 
        new_exit_block
    );
}

[[nodiscard]] ExpressionsAndStatementsLLVMTranslator 
ExpressionsAndStatementsLLVMTranslator::create_translator_for_nested_conditional(){
    return create_translator_for_nested_loop(loop_entry_block, loop_exit_block);
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_statement_into_llvm(
    llvm::BasicBlock* block,
    const Statement& statement
){
    switch (statement.statement_kind()) {
        case StatementBody::Kind::assignment: return translate_assignment_into_llvm(block, statement.get<Assignment>());
        case StatementBody::Kind::variable_declaration: return translate_variable_declaration_into_llvm(block, statement.get<VariableDeclaration>());
        case StatementBody::Kind::const_declaration: return translate_constant_declaration_into_llvm(block, statement.get<ConstDeclaration>());
        case StatementBody::Kind::function_call: return translate_void_function_call_into_llvm(block, statement.get<FunctionCall>());
        case StatementBody::Kind::return_statement: return translate_return_statement_into_llvm(block, statement.get<Return>());
        case StatementBody::Kind::break_statement: return translate_break_statement_into_llvm(block, statement.get<Break>());
        case StatementBody::Kind::continue_statement: return translate_continue_statement_into_llvm(block, statement.get<Continue>());
        case StatementBody::Kind::conditional: return translate_conditional_into_llvm(block, statement.get<Conditional>());
        case StatementBody::Kind::while_loop: return translate_while_loop_into_llvm(block, statement.get<WhileLoop>());
        case StatementBody::Kind::until_loop: return translate_until_loop_into_llvm(block, statement.get<UntilLoop>());
    }
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expression_into_llvm(
    llvm::BasicBlock* block, 
    const Expression& expression
) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::array_literal:          return translate_array_literal_into_llvm(block, expression.get<ArrayLiteral>());
        case ExpressionBody::Kind::binary_operator:        return translate_binary_operator_into_llvm(block, expression.get<BinaryOperator>());
        case ExpressionBody::Kind::bool_literal:           return translate_boolean_literal_into_llvm(block, expression.get<BoolLiteral>());
        case ExpressionBody::Kind::char_literal:           return translate_character_literal_into_llvm(block, expression.get<CharLiteral>());
        case ExpressionBody::Kind::float_literal:          return translate_floating_literal_into_llvm(block, expression.get<FloatLiteral>());
        case ExpressionBody::Kind::function_call:          return translate_expr_function_call_into_llvm(block, expression.get<FunctionCall>());
        case ExpressionBody::Kind::identifier:             return translate_identifier_into_llvm(block, expression.get<Identifier>());
        case ExpressionBody::Kind::int_literal:            return translate_integer_literal_into_llvm(block, expression.get<IntLiteral>());
        case ExpressionBody::Kind::string_literal:         return translate_string_literal_into_llvm(block, expression.get<StringLiteral>());
        case ExpressionBody::Kind::type_operator:          return translate_type_operator_into_llvm(block, expression.get<TypeOperator>());
        case ExpressionBody::Kind::unary_operator:         return translate_unary_operator_into_llvm(block, expression.get<UnaryOperator>());
        case ExpressionBody::Kind::dot_member_access:      return translate_dot_member_access_into_llvm(block, expression.get<DotMemberAccess>());
        case ExpressionBody::Kind::square_bracket_access:  return translate_square_bracket_access_into_llvm(block, expression.get<SquareBracketsAccess>());
    }
    assert_unreachable();
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expr_function_call_into_llvm(
    llvm::BasicBlock* block,
    const FunctionCall& fcall
) {
    CallableCodeBlock ccb = program_representation.resolve_function_call(fcall, scope_context.raw_scope_context);
    llvm::Function* llvm_function = callable_codeblocks_llvm_translator.translate_callable_code_block_into_llvm(ccb);
    llvm::IRBuilder<> builder(block);
    return builder.CreateCall(llvm_function);
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_void_function_call_into_llvm(
    llvm::BasicBlock* block,
    const FunctionCall& function_call
) {
    std::ignore = translate_expr_function_call_into_llvm(block, function_call);
    return block;
}