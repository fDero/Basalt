
#pragma once
#include "language/typesystem.hpp"
#include <vector>
#include <string>

struct ExpressionBody : public DebugInformationsAwareEntity {

    ExpressionBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }
};

class Expression : public Polymorph<ExpressionBody> {
    
    public:

        using Polymorph<ExpressionBody>::is;
        using Polymorph<ExpressionBody>::get;
        using Polymorph<ExpressionBody>::Polymorph;

        [[nodiscard]] bool is_wrapped_in_in_parenthesis() const { return wrapped_in_parenthesis; }
        void wrap_in_parenthesis() { wrapped_in_parenthesis = true; }

    private:
        bool wrapped_in_parenthesis = false;
};

struct ArrayLiteral : public ExpressionBody {

    ArrayLiteral(
        int length,
        const TypeSignature& type,
        const std::vector<Expression>& elements,
        const DebugInformationsAwareEntity& debug_info
    );

    int array_length;
    TypeSignature stored_type;
    std::vector<Expression> elements;

};

struct TypeOperator : public ExpressionBody {

    TypeOperator(
        const Token& operator_token,
        const Expression& expression, 
        const TypeSignature& typesignature
    );

    std::string operator_text;
    Expression expression;
    TypeSignature typesignature;

    [[nodiscard]] Token as_token() const;
};


struct BinaryOperator : public ExpressionBody {

    BinaryOperator(
        const Token& operator_token, 
        const Expression& lx, 
        const Expression& rx
    );

    std::string operator_text;
    Expression left_operand;
    Expression right_operand;

    [[nodiscard]] Token as_token() const;
};

struct UnaryOperator : public ExpressionBody {

    UnaryOperator(
        const Token& operator_token, 
        const Expression& expr
    );
    
    std::string operator_text;
    Expression operand;
    [[nodiscard]] Token as_token() const;
};

struct Identifier : public ExpressionBody {
    Identifier(const Token& identifier_token);
    std::string name;
};

struct StringLiteral : public ExpressionBody {
    StringLiteral(const Token& string_literal_token);
    std::string value;
};

struct IntLiteral : public ExpressionBody {
    IntLiteral(const Token& int_literal_token);
    int value;
};

struct FloatLiteral : public ExpressionBody {
    FloatLiteral(const Token& float_literal_token);
    double value;
};

struct BoolLiteral : public ExpressionBody {
    BoolLiteral(const Token& bool_literal_token);
    bool value;
};

struct CharLiteral : public ExpressionBody {
    CharLiteral(const Token& char_literal_token);
    char value;
};