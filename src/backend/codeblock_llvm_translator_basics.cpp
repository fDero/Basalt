//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/codeblock_llvm_translator.hpp"

CodeBlockLLVMTranslator::CodeBlockLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    ScopeContext& scope_context,
    llvm::LLVMContext& context,
    llvm::IRBuilder<>& builder
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
    , scope_context(scope_context)
    , context(context)
    , builder(builder)
{ }

void CodeBlockLLVMTranslator::translate_statement_into_llvm(
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

llvm::Value* CodeBlockLLVMTranslator::translate_expression_into_llvm(const Expression& expression) {
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
    }
}