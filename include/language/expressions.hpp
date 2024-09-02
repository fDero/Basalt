/**
 * @file expressions.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the different kinds of expressions in the language.
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <vector>
#include <string>

#include "language/typesignatures.hpp"
#include "frontend/syntax.hpp"

/**
 * @brief   Real base class for all the expressions in the language.
 * 
 * @details Base class for all the expressions in the language. Used togheter
 *          with the Expression class to create a value-semantics wrapper around
 *          the different kinds of expressions in a polymorphic way.
 * 
 * @note    This struct is not meant to be used directly, instead you should use the Expression class,
 *          for wich this class is just the internal implementation.
 * 
 * @note    Since this struct extends DebugInformationsAwareEntity, all the expressions in the language
 *          are aware of their position in the source code file.
 *          
 * @see     Expression
 * @see     Polymorph
 */
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
        bool_literal,
        dot_member_access,
        square_bracket_access
    };

    virtual Kind expression_kind() const = 0;

    virtual ~ExpressionBody() = default;

    ExpressionBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }
};

/**
 * @brief   Value-semantics polymorphism class to represent all the expressions in the language.
 * 
 * @details the Expression class is used togheter with the ExpressionBody class to 
 *          create a value-semantics wrapper around the different kinds of expressions in a polymorphic way.
 * 
 * @see     ExpressionBody
 * @see     Polymorph
 */
class Expression : public Polymorph<ExpressionBody> {
    
    public:

        using Polymorph<ExpressionBody>::is;
        using Polymorph<ExpressionBody>::get;
        using Polymorph<ExpressionBody>::Polymorph;

        void wrap_in_parenthesis() { wrapped_in_parenthesis = true; }
        [[nodiscard]] bool is_wrapped_in_in_parenthesis() const { return wrapped_in_parenthesis; }
        [[nodiscard]] ExpressionBody::Kind expression_kind() const;

        const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const {
            return ptr->as_debug_informations_aware_entity();
        }

    private:
        bool wrapped_in_parenthesis = false;
};

/**
 * @brief   Used to represent the use of the dot operator to access
 *          a member of a struct in the source code.
 * 
 * @details The DotMemberAccess struct is used to represent the use of the dot operator
 *          to access a member of a struct in the source code. It contains the expression
 *          that evaluates to the struct, and the name of the member to access.
 * 
 * @see     Expression
 * @see     StructDefinition
 */
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

/**
 * @brief   Used to represent the use of the square brackets operator
 *          to access an element of an array in the source code.
 * 
 * @details The SquareBracketsAccess struct is used to represent the use of the square brackets operator
 *          to access an element of an array in the source code. It contains the expression that evaluates
 *          to the array, and the expression that evaluates to the index of the element to access.
 * 
 * @see     ArrayType
 * @see     ArrayLiteral
 */
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

/**
 * @brief   Used to represent an inline array-literal in the source code.
 * 
 * @details The ArrayLiteral struct is used to represent an inline array-literal in the source code.
 *          It contains the length of the array, the type of the elements stored in the array,
 *          and the expressions that evaluate to the elements of the array.
 * 
 * @see     ArrayType
 * @see     SquareBracketsAccess
 */
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

/**
 * @brief   Used to represent a type-query or a type-cast in the source code.
 * 
 * @details The TypeOperator struct is used to represent a type-query or a type-cast in the source code.
 *          It contains the expression that evaluates to the value to cast or query, and the type-signature
 *          that represents the type to cast or query.
 *          
 */
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

/**
 * @brief   Used to represent a binary operator in the source code.
 * 
 * @details The BinaryOperator struct is used to represent a binary operator in the source code.
 *          It contains the operator token, the expression that evaluates to the left operand,
 *          and the expression that evaluates to the right operand.
 */
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

/**
 * @brief   Used to represent a unary operator in the source code.
 * 
 * @details The UnaryOperator struct is used to represent a unary operator in the source code.
 *          It contains the operator token, and the expression that evaluates to the operand.
 */
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

/**
 * @brief   Used to represent the name of a function, a variable or a constant in the source code.
 * 
 * @details The Identifier struct is used to represent the name of a function, a variable or a 
 *          constant in the source code. It Just contains the name of the identifier, togheter with
 *          the token that represents it.
 */
struct Identifier : public ExpressionBody {

    virtual ~Identifier() = default;

    Identifier(const Token& identifier_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    std::string name;
};

/**
 * @brief   Used to represent a string-literal in the source code.
 * 
 * @details The StringLiteral struct is used to represent a string-literal in the source code.
 *          It contains the value of the string-literal, togheter with the token that represents it.
 */
struct StringLiteral : public ExpressionBody {

    virtual ~StringLiteral() = default;

    StringLiteral(const Token& string_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    std::string value;
};

/**
 * @brief   Used to represent an integer-literal in the source code.
 * 
 * @details The IntLiteral struct is used to represent an integer-literal in the source code.
 *          It contains the value of the integer-literal, togheter with the token that represents it.
 */
struct IntLiteral : public ExpressionBody {

    virtual ~IntLiteral() = default;

    IntLiteral(const Token& int_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    int value;
};

/**
 * @brief   Used to represent a float-literal in the source code.
 * 
 * @details The FloatLiteral struct is used to represent a float-literal in the source code.
 *          It contains the value of the float-literal, togheter with the token that represents it.
 */
struct FloatLiteral : public ExpressionBody {

    virtual ~FloatLiteral() = default;

    FloatLiteral(const Token& float_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    double value;
};

/**
 * @brief   Used to represent a boolean-literal in the source code.
 * 
 * @details The BoolLiteral struct is used to represent a boolean-literal in the source code.
 *          It contains the value of the boolean-literal, togheter with the token that represents it.
 */
struct BoolLiteral : public ExpressionBody {

    virtual ~BoolLiteral() = default;

    BoolLiteral(const Token& bool_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;
    
    bool value;
};

/**
 * @brief   Used to represent a char-literal in the source code.
 * 
 * @details The CharLiteral struct is used to represent a char-literal in the source code.
 *          It contains the value of the char-literal, togheter with the token that represents it.
 */
struct CharLiteral : public ExpressionBody {

    virtual ~CharLiteral() = default;

    CharLiteral(const Token& char_literal_token);
    
    [[nodiscard]] ExpressionBody::Kind expression_kind() const override;

    char value;
};