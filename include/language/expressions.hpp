//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>

#include "language/typesignatures.hpp"
#include "frontend/syntax.hpp"

struct ExpressionBody : public DebugInformationsAwareEntity {

    enum class Kind {
        function_call,
        type_operator,
        binary_operator,
        unary_operator,
        identifier,
        array_literal,
        string_literal,
        float_literal,
        char_literal,
        int_literal,
        bool_literal,
        dot_member_access,
        square_bracket_access
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

        void wrap_in_parenthesis();

        [[nodiscard]] bool is_wrapped_in_in_parenthesis() const;
        [[nodiscard]] bool is_literal() const;
        [[nodiscard]] ExpressionBody::Kind expression_kind() const;
        [[nodiscard]] const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const;

    private:
        bool wrapped_in_parenthesis = false;
};

struct DotMemberAccess : public ExpressionBody {

    virtual ~DotMemberAccess() = default;

    DotMemberAccess(
        const DebugInformationsAwareEntity& debug_info,
        const Expression& struct_value,
        const std::string& member_name
    );

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    Expression struct_value;
    std::string member_name;
};

struct SquareBracketsAccess : public ExpressionBody {

    virtual ~SquareBracketsAccess() = default;

    SquareBracketsAccess(
        const DebugInformationsAwareEntity& debug_info,
        const Expression& storage,
        const Expression& index
    );

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    Expression storage;
    Expression index;
};

struct ArrayLiteral : public ExpressionBody {

    virtual ~ArrayLiteral() = default;

    ArrayLiteral(
        std::optional<size_t> length,
        const TypeSignature& type,
        const std::vector<Expression>& elements,
        const DebugInformationsAwareEntity& debug_info
    );

    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    std::optional<size_t> array_length;
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

    TypeOperator(
        const DebugInformationsAwareEntity& debug_info,
        const std::string& operator_text,
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
        const DebugInformationsAwareEntity& debug_info, 
        const std::string& operator_text,
        const Expression& expr
    );

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
    Identifier(const DebugInformationsAwareEntity& debug_info, const std::string& name);

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