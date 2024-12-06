//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/expressions.hpp"

ExpressionBody::Kind Expression::expression_kind() const {
    return ptr->expression_kind();
}

ExpressionBody::Kind SquareBracketsAccess::expression_kind() const {
    return ExpressionBody::Kind::square_bracket_access;
}

ExpressionBody::Kind DotMemberAccess::expression_kind() const {
    return ExpressionBody::Kind::dot_member_access;
}

ExpressionBody::Kind ArrayLiteral::expression_kind() const {
    return ExpressionBody::Kind::array_literal;
}

ExpressionBody::Kind TypeOperator::expression_kind() const {
    return ExpressionBody::Kind::type_operator;
}

ExpressionBody::Kind BinaryOperator::expression_kind() const {
    return ExpressionBody::Kind::binary_operator;
}

ExpressionBody::Kind UnaryOperator::expression_kind() const {
    return ExpressionBody::Kind::unary_operator;
}

ExpressionBody::Kind Identifier::expression_kind() const {
    return ExpressionBody::Kind::identifier;
}

ExpressionBody::Kind StringLiteral::expression_kind() const {
    return ExpressionBody::Kind::string_literal;
}

ExpressionBody::Kind IntLiteral::expression_kind() const {
    return ExpressionBody::Kind::int_literal;
}

ExpressionBody::Kind FloatLiteral::expression_kind() const {
    return ExpressionBody::Kind::float_literal;
}

ExpressionBody::Kind BoolLiteral::expression_kind() const {
    return ExpressionBody::Kind::bool_literal;
}

ExpressionBody::Kind CharLiteral::expression_kind() const {
    return ExpressionBody::Kind::char_literal;
}
