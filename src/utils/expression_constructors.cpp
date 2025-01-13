//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/expressions.hpp"
#include "language/functions.hpp"

SquareBracketsAccess::SquareBracketsAccess(
    const DebugInformationsAwareEntity& square_brackets_token, 
    const Expression& storage, 
    const Expression& index
)
    : ExpressionBody(square_brackets_token)
    , storage(storage)
    , index(index) 
{}

DotMemberAccess::DotMemberAccess(
    const DebugInformationsAwareEntity& dot_token, 
    const Expression& struct_value, 
    const std::string& member_name
)
    : ExpressionBody(dot_token)
    , struct_value(struct_value)
    , member_name(member_name) 
{}

ArrayLiteral::ArrayLiteral(
    std::optional<size_t> length,
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

TypeOperator::TypeOperator(
    const DebugInformationsAwareEntity& debug_info,
    const std::string& operator_text,
    const Expression& expression, 
    const TypeSignature& typesignature
)
    : ExpressionBody(debug_info)
    , operator_text(operator_text)
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
    const DebugInformationsAwareEntity& debug_info, 
    const std::string& operator_text,
    const Expression& expr
)
    : ExpressionBody(debug_info)
    , operator_text(operator_text)
    , operand(expr) 
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

Identifier::Identifier(const DebugInformationsAwareEntity& debug_info, const std::string& name)
    : ExpressionBody(debug_info) 
    , name(name) 
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
    const std::string& package_prefix,
    const std::vector<Expression>& args, 
    const std::vector<TypeSignature>& instantiated_generics
) 
    : ExpressionBody(function_call_token)
    , StatementBody(function_call_token)
    , package_prefix(package_prefix)
    , function_name(function_call_token.sourcetext)
    , instantiated_generics(instantiated_generics)
    , arguments(args) 
{ }

FunctionCall::FunctionCall(
    const Token& function_call_token, 
    const std::vector<Expression>& args, 
    const std::vector<TypeSignature>& instantiated_generics
) 
    : ExpressionBody(function_call_token)
    , StatementBody(function_call_token)
    , package_prefix("")
    , function_name(function_call_token.sourcetext)
    , instantiated_generics(instantiated_generics)
    , arguments(args) 
{ }