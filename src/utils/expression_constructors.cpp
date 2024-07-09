
#include "language/expressions.hpp"
#include "language/functions.hpp"

ArrayLiteral::ArrayLiteral(
    int length,
    const TypeSignature& type,
    const std::vector<Expression>& elements,
    const DebugInformationsAwareEntity& debug_info
)
    : ExpressionBody(debug_info)
    , array_length(length)
    , stored_type(type)
    , elements(elements)
{}

TypeOperator::TypeOperator(
    const Token& operator_token,
    const Expression& expression, 
    const TypeSignature& typesignature
)
    : ExpressionBody(operator_token)
    , operator_text(operator_token.sourcetext)
    , expression(expression)
    , typesignature(typesignature)
{}

BinaryOperator::BinaryOperator(
    const Token& operator_token, 
    const Expression& lx, 
    const Expression& rx
)
    : ExpressionBody(operator_token)
    , operator_text(operator_token.sourcetext)
    , left_operand(lx)
    , right_operand(rx) 
{}

UnaryOperator::UnaryOperator(
    const Token& operator_token, 
    const Expression& expr
)
    : ExpressionBody(operator_token)
    , operator_text(operator_token.sourcetext)
    , operand(expr) 
{}

Identifier::Identifier(const Token& identifier_token)
    : ExpressionBody(identifier_token) 
    , name(identifier_token.sourcetext) 
{}

StringLiteral::StringLiteral(const Token& string_literal_token)
    : ExpressionBody(string_literal_token)
    , value(string_literal_token.sourcetext) 
{ }

IntLiteral::IntLiteral(const Token& int_literal_token)
    : ExpressionBody(int_literal_token)
    , value(std::stoi(int_literal_token.sourcetext)) 
{ }

FloatLiteral::FloatLiteral(const Token& float_literal_token)
    : ExpressionBody(float_literal_token)
    , value(std::stod(float_literal_token.sourcetext)) 
{ }

BoolLiteral::BoolLiteral(const Token& bool_literal_token)
    : ExpressionBody(bool_literal_token)
    , value(bool_literal_token.sourcetext == "true") 
{ }

CharLiteral::CharLiteral(const Token& char_literal_token)
    : ExpressionBody(char_literal_token)
    , value(char_literal_token.sourcetext[1]) 
{ }

FunctionCall::FunctionCall(
    const Token& function_call_token, 
    const std::vector<Expression>& args, 
    const std::vector<TypeSignature>& instantiated_generics
) 
    : ExpressionBody(function_call_token)
    , StatementBody(function_call_token)
    , function_name(function_call_token.sourcetext)
    , instantiated_generics(instantiated_generics)
    , arguments(args) 
{ }