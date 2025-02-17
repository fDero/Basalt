//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/tokenizer.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "syntax/infixes.hpp"
#include "syntax/prefixes.hpp"
#include <map>

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
{
    static std::map<std::string, Kind> kinds{
        { boolean_and_operator,       Kind::boolean_and       },
        { boolean_or_operator,        Kind::boolean_or        },
        { boolean_xor_operator,       Kind::boolean_xor       },
        { cmp_lt_operator,            Kind::cmp_lt            },
        { cmp_gt_operator,            Kind::cmp_gt            },
        { cmp_leq_operator,           Kind::cmp_leq           },
        { cmp_geq_operator,           Kind::cmp_geq           },
        { cmp_eq_operator,            Kind::cmp_eq            },
        { cmp_neq_operator,           Kind::cmp_neq           },
        { math_infix_sum_operator,    Kind::math_sum          },
        { math_infix_sub_operator,    Kind::math_sub          },
        { math_infix_mul_operator,    Kind::math_mul          },
        { math_infix_div_operator,    Kind::math_div          },
        { math_infix_mod_operator,    Kind::math_mod          },
    };
    binary_op_kind = kinds[operator_text];
}

UnaryOperator::UnaryOperator(
    const DebugInformationsAwareEntity& debug_info, 
    const std::string& operator_text,
    const Expression& expr
)
    : ExpressionBody(debug_info)
    , operator_text(operator_text)
    , operand(expr) 
{
    static std::map<std::string, Kind> kinds {
        { boolean_not_operator,         Kind::boolean_not         },
        { prefix_minus_sign,            Kind::minus_sign          },
        { prefix_plus_sign,             Kind::plus_sign           },
        { pointer_dereference_operator, Kind::pointer_dereference },
        { address_operator,             Kind::address_of          },
    };
    unary_op_kind = kinds[operator_text];
}

UnaryOperator::UnaryOperator(
    const Token& operator_token, 
    const Expression& expr
)
    : UnaryOperator(
        operator_token, 
        operator_token.sourcetext, 
        expr
    )
{}

Identifier::Identifier(const Token& identifier_token)
    : ExpressionBody(identifier_token) 
    , name(identifier_token.sourcetext) 
{}

Identifier::Identifier(const DebugInformationsAwareEntity& debug_info, const std::string& name)
    : ExpressionBody(debug_info) 
    , name(name) 
{}

StringLiteral::StringLiteral(
    const Token& string_literal_token,
    const std::string& value
)
    : ExpressionBody(string_literal_token)
    , value(value) 
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

CharLiteral::CharLiteral(
    const Token& char_literal_token,
    char value
)
    : ExpressionBody(char_literal_token)
    , value(value) 
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