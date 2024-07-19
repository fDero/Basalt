
#include "language/expressions.hpp"

[[nodiscard]] ExpressionBody::Kind Expression::expression_kind() const {
    return ptr->expression_kind();
}

[[nodiscard]] ExpressionBody::Kind ArrayLiteral::expression_kind() const {
    return ExpressionBody::Kind::array_literal;
}

[[nodiscard]] ExpressionBody::Kind TypeOperator::expression_kind() const {
    return ExpressionBody::Kind::type_operator;
}

[[nodiscard]] ExpressionBody::Kind BinaryOperator::expression_kind() const {
    return ExpressionBody::Kind::binary_operator;
}

[[nodiscard]] ExpressionBody::Kind UnaryOperator::expression_kind() const {
    return ExpressionBody::Kind::unary_operator;
}

[[nodiscard]] ExpressionBody::Kind Identifier::expression_kind() const {
    return ExpressionBody::Kind::identifier;
}

[[nodiscard]] ExpressionBody::Kind StringLiteral::expression_kind() const {
    return ExpressionBody::Kind::string_literal;
}

[[nodiscard]] ExpressionBody::Kind IntLiteral::expression_kind() const {
    return ExpressionBody::Kind::int_literal;
}

[[nodiscard]] ExpressionBody::Kind FloatLiteral::expression_kind() const {
    return ExpressionBody::Kind::float_literal;
}

[[nodiscard]] ExpressionBody::Kind BoolLiteral::expression_kind() const {
    return ExpressionBody::Kind::bool_literal;
}

[[nodiscard]] ExpressionBody::Kind CharLiteral::expression_kind() const {
    return ExpressionBody::Kind::char_literal;
}
