
#pragma once
#include "language/typesystem.hpp"
#include <vector>
#include <string>

struct ExpressionBody {

    virtual ~ExpressionBody() = default;
};

class Expression : public Polymorph<ExpressionBody> {
    
    public:

        using Polymorph<ExpressionBody>::is;
        using Polymorph<ExpressionBody>::get;
        using Polymorph<ExpressionBody>::Polymorph;

        bool is_compiletime_evaluation_possibile() const { return compiletime_avialable; }
        bool is_wrapped_in_in_parenthesis() const { return wrapped_in_parenthesis; }
        void wrap_in_parenthesis() { wrapped_in_parenthesis = true; }
        void mark_as_compiletime_avialable() { compiletime_avialable = true; }

    private:
        bool wrapped_in_parenthesis = false;
        bool compiletime_avialable = false;
};

struct ArrayLiteral : public ExpressionBody {
    int array_length;
    TypeSignature stored_type;
    std::vector<Expression> elements;
    
    virtual ~ArrayLiteral() = default;

    ArrayLiteral(
        int length,
        const TypeSignature& type,
        const std::vector<Expression>& elements
    )
        : array_length(length), stored_type(type), elements(elements) {}
};

struct BinaryOperator : public ExpressionBody {
    std::string operator_text;
    Expression left_operand;
    Expression right_operand;
    BinaryOperator(const std::string& opertext, const Expression& lx, const Expression& rx)
        : operator_text(opertext), left_operand(lx), right_operand(rx) {}

    virtual ~BinaryOperator() = default;
};

struct UnaryOperator : public ExpressionBody {
    std::string operator_text;
    Expression operand;
    UnaryOperator(
        const std::string& opertext, 
        const Expression& expr
    )
        : operator_text(opertext), operand(expr) {}

    virtual ~UnaryOperator() = default;
};

struct Identifier : public ExpressionBody {
    std::string name;
    Identifier(const std::string& str)
        : name(str) {}

    virtual ~Identifier() = default;
};

struct StringLiteral : public ExpressionBody {
    std::string value;
    StringLiteral(const std::string& str)
        : value(str) { }

    virtual ~StringLiteral() = default;
};

struct IntLiteral : public ExpressionBody {
    int value;
    IntLiteral(int number)
        : value(number) { }

    virtual ~IntLiteral() = default;
};

struct FloatLiteral : public ExpressionBody {
    double value;
    FloatLiteral(const double number)
        : value(number) { }
    
    virtual ~FloatLiteral() = default;
};

struct BoolLiteral : public ExpressionBody {
    bool value;
    BoolLiteral(const bool boolean)
        : value(boolean) { }

    virtual ~BoolLiteral() = default;
};

struct CharLiteral : public ExpressionBody {
    char value;
    CharLiteral(const char character)
        : value(character) { }

    virtual ~CharLiteral() = default;
};