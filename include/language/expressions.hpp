
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

        [[nodiscard]] ExpressionBody::Kind expression_kind() const {
            return ptr->expression_kind();
        }

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

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::array_literal;
    }

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

    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::type_operator;
    }

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

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::binary_operator;
    }

    [[nodiscard]] Token as_token() const;
};

struct UnaryOperator : public ExpressionBody {

    UnaryOperator(
        const Token& operator_token, 
        const Expression& expr
    );

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::unary_operator;
    }
    
    std::string operator_text;
    Expression operand;
    [[nodiscard]] Token as_token() const;
};

struct Identifier : public ExpressionBody {
    Identifier(const Token& identifier_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::identifier;
    }

    std::string name;
};

struct StringLiteral : public ExpressionBody {
    StringLiteral(const Token& string_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::string_literal;
    }

    std::string value;
};

struct IntLiteral : public ExpressionBody {
    IntLiteral(const Token& int_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::int_literal;
    }
    
    int value;
};

struct FloatLiteral : public ExpressionBody {
    FloatLiteral(const Token& float_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::float_literal;
    }
    
    double value;
};

struct BoolLiteral : public ExpressionBody {
    BoolLiteral(const Token& bool_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::bool_literal;
    }
    
    bool value;
};

struct CharLiteral : public ExpressionBody {
    CharLiteral(const Token& char_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override {
        return ExpressionBody::Kind::char_literal;
    }

    char value;
};