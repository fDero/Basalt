
#pragma once
#include "language/typesystem.hpp"
#include <vector>
#include <string>

struct ExpressionBody : public DebugInformationsAwareEntity {

    ExpressionBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }

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

    virtual ~ArrayLiteral() = default;
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

struct BinaryOperator : public ExpressionBody {
    
    virtual ~BinaryOperator() = default;
    BinaryOperator(
        const Token& operator_token, 
        const Expression& lx, 
        const Expression& rx
    );

    std::string operator_text;
    Expression left_operand;
    Expression right_operand;

    Token as_token() const;
};

struct UnaryOperator : public ExpressionBody {
    
    virtual ~UnaryOperator() = default;
    UnaryOperator(
        const Token& operator_token, 
        const Expression& expr
    );
    
    std::string operator_text;
    Expression operand;
    
    Token as_token() const;
};

struct Identifier : public ExpressionBody {

    virtual ~Identifier() = default;
    Identifier(const Token& identifier_token);

    std::string name;
};

struct StringLiteral : public ExpressionBody {

    virtual ~StringLiteral() = default;
    StringLiteral(const Token& string_literal_token);

    std::string value;
};

struct IntLiteral : public ExpressionBody {
    
    virtual ~IntLiteral() = default;
    IntLiteral(const Token& int_literal_token);
    
    int value;
};

struct FloatLiteral : public ExpressionBody {
    
    virtual ~FloatLiteral() = default;
    FloatLiteral(const Token& float_literal_token);
    
    double value;
};

struct BoolLiteral : public ExpressionBody {

    virtual ~BoolLiteral() = default;
    BoolLiteral(const Token& bool_literal_token);

    bool value;
};

struct CharLiteral : public ExpressionBody {
    
    virtual ~CharLiteral() = default;
    CharLiteral(const Token& char_literal_token);

    char value;
};