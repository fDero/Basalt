
#pragma once
#include "language/typesystem.hpp"
#include <vector>
#include <string>

struct ExpressionBody : public DebugInformationsAwareEntity {

    enum class Kind {
        function_call,
        array_literal,
        type_operator,
        binary_operator,
        unary_operator,
        identifier,
        string_literal,
        float_literal,
        char_literal,
        int_literal,
        bool_literal
    };

    virtual Kind expression_kind() const = 0;

    virtual ~ExpressionBody() = default;

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
        [[nodiscard]] ExpressionBody::Kind expression_kind() const;

    private:
        bool wrapped_in_parenthesis = false;
};

struct ArrayLiteral : public ExpressionBody {

    virtual ~ArrayLiteral() = default;

    ArrayLiteral(
        int length,
        const TypeSignature& type,
        const std::vector<Expression>& elements,
        const DebugInformationsAwareEntity& debug_info
    );

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    int array_length;
    TypeSignature stored_type;
    std::vector<Expression> elements;

};

struct TypeOperator : public ExpressionBody {

    virtual ~TypeOperator() = default;

    TypeOperator(
        const Token& operator_token,
        const Expression& expression, 
        const TypeSignature& typesignature
    );

    [[nodiscard]] Token as_token() const;
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    std::string operator_text;
    Expression expression;
    TypeSignature typesignature;
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

    [[nodiscard]] Token as_token() const;
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
};

struct UnaryOperator : public ExpressionBody {

    virtual ~UnaryOperator() = default;

    UnaryOperator(
        const Token& operator_token, 
        const Expression& expr
    );

    [[nodiscard]] Token as_token() const;
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    std::string operator_text;
    Expression operand;
};

struct Identifier : public ExpressionBody {

    virtual ~Identifier() = default;

    Identifier(const Token& identifier_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    std::string name;
};

struct StringLiteral : public ExpressionBody {

    virtual ~StringLiteral() = default;

    StringLiteral(const Token& string_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    std::string value;
};

struct IntLiteral : public ExpressionBody {

    virtual ~IntLiteral() = default;

    IntLiteral(const Token& int_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    int value;
};

struct FloatLiteral : public ExpressionBody {

    virtual ~FloatLiteral() = default;

    FloatLiteral(const Token& float_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    double value;
};

struct BoolLiteral : public ExpressionBody {

    virtual ~BoolLiteral() = default;

    BoolLiteral(const Token& bool_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    bool value;
};

struct CharLiteral : public ExpressionBody {

    virtual ~CharLiteral() = default;

    CharLiteral(const Token& char_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    char value;
};