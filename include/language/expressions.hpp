
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
    
    int array_length;
    TypeSignature stored_type;
    std::vector<Expression> elements;

    virtual ~ArrayLiteral() = default;

    ArrayLiteral(
        int length,
        const TypeSignature& type,
        const std::vector<Expression>& elements,
        const DebugInformationsAwareEntity& debug_info
    )
        : ExpressionBody(debug_info)
        , array_length(length)
        , stored_type(type)
        , elements(elements)
    {}
};

struct BinaryOperator : public ExpressionBody {
    
    std::string operator_text;
    Expression left_operand;
    Expression right_operand;

    virtual ~BinaryOperator() = default;

    BinaryOperator(
        const Token& operator_token, 
        const Expression& lx, 
        const Expression& rx
    )
        : ExpressionBody(operator_token)
        , operator_text(operator_token.sourcetext)
        , left_operand(lx)
        , right_operand(rx) 
    {}

    Token as_token() const {
        return Token { 
            operator_text, 
            DebugInformationsAwareEntity::filename,
            DebugInformationsAwareEntity::line_number,
            DebugInformationsAwareEntity::tok_number,
            DebugInformationsAwareEntity::char_pos,
            Token::Type::symbol
        };
    }
};

struct UnaryOperator : public ExpressionBody {
    
    std::string operator_text;
    Expression operand;

    virtual ~UnaryOperator() = default;

    UnaryOperator(
        const Token& operator_token, 
        const Expression& expr
    )
        : ExpressionBody(operator_token)
        , operator_text(operator_token.sourcetext)
        , operand(expr) 
    {}

    Token as_token() const {
        return Token { 
            operator_text, 
            DebugInformationsAwareEntity::filename,
            DebugInformationsAwareEntity::line_number,
            DebugInformationsAwareEntity::tok_number,
            DebugInformationsAwareEntity::char_pos,
            Token::Type::symbol
        };
    }
};

struct Identifier : public ExpressionBody {

    std::string name;

    virtual ~Identifier() = default;

    Identifier(const Token& identifier_token)
        : ExpressionBody(identifier_token) 
        , name(identifier_token.sourcetext) 
    {}
};

struct StringLiteral : public ExpressionBody {

    std::string value;

    virtual ~StringLiteral() = default;

    StringLiteral(const Token& string_literal_token)
        : ExpressionBody(string_literal_token)
        , value(string_literal_token.sourcetext) 
    { }
};

struct IntLiteral : public ExpressionBody {
    
    int value;

    virtual ~IntLiteral() = default;

    IntLiteral(const Token& int_literal_token)
        : ExpressionBody(int_literal_token)
        , value(std::stoi(int_literal_token.sourcetext)) 
    { }
};

struct FloatLiteral : public ExpressionBody {
    
    double value;

    virtual ~FloatLiteral() = default;

    FloatLiteral(const Token& float_literal_token)
        : ExpressionBody(float_literal_token)
        , value(std::stod(float_literal_token.sourcetext)) 
    { }
};

struct BoolLiteral : public ExpressionBody {
    bool value;

    virtual ~BoolLiteral() = default;

    BoolLiteral(const Token& bool_literal_token)
        : ExpressionBody(bool_literal_token)
        , value(bool_literal_token.sourcetext == "true") 
    { }
};

struct CharLiteral : public ExpressionBody {
    
    char value;

    virtual ~CharLiteral() = default;

    CharLiteral(const Token& char_literal_token)
        : ExpressionBody(char_literal_token)
        , value(char_literal_token.sourcetext[1]) 
    { }
};