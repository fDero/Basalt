
#pragma once
#include "language/typesystem.hpp"
#include <vector>
#include <string>

struct ScopedData;
struct ProgramRappresentation;

struct ExpressionBody {
    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const = 0;

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

        [[nodiscard]] TypeSignature get_type(
            const ProgramRappresentation& program, const ScopedData& data) const {
                return ptr->get_type(program, data);
            };

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

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct BinaryOperator : public ExpressionBody {
    std::string operator_text;
    Expression left_operand;
    Expression right_operand;
    BinaryOperator(const std::string& opertext, const Expression& lx, const Expression& rx)
        : operator_text(opertext), left_operand(lx), right_operand(rx) {}

    virtual ~BinaryOperator() = default;

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;

    [[nodiscard]] bool is_valid_comparison(
        const BinaryOperator& op,  const ProgramRappresentation& program,
        const TypeSignature& left, const TypeSignature& right) const;

    [[nodiscard]] bool is_numerical_operator() const;
    [[nodiscard]] bool is_comparison_operator() const;
    [[nodiscard]] bool is_logical_operator() const;

    [[nodiscard]] bool has_int_operands(const TypeSignature& left, const TypeSignature& right) const;
    [[nodiscard]] bool has_numerical_operands(const TypeSignature& left, const TypeSignature& right) const;
    [[nodiscard]] bool has_string_operands(const TypeSignature& left, const TypeSignature& right) const;
    [[nodiscard]] bool has_boolean_operands(const TypeSignature& left, const TypeSignature& right) const;
    [[nodiscard]] bool is_string_operator() const;
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

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct Identifier : public ExpressionBody {
    std::string name;
    Identifier(const std::string& str)
        : name(str) {}

    virtual ~Identifier() = default;

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct StringLiteral : public ExpressionBody {
    std::string value;
    StringLiteral(const std::string& str)
        : value(str) { }

    virtual ~StringLiteral() = default;

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct IntLiteral : public ExpressionBody {
    int value;
    IntLiteral(int number)
        : value(number) { }

    virtual ~IntLiteral() = default;

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct FloatLiteral : public ExpressionBody {
    double value;
    FloatLiteral(const double number)
        : value(number) { }
    
    virtual ~FloatLiteral() = default;

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct BoolLiteral : public ExpressionBody {
    bool value;
    BoolLiteral(const bool boolean)
        : value(boolean) { }

    virtual ~BoolLiteral() = default;

    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};

struct CharLiteral : public ExpressionBody {
    char value;
    CharLiteral(const char character)
        : value(character) { }

    virtual ~CharLiteral() = default;
    
    [[nodiscard]] virtual TypeSignature get_type(
        const ProgramRappresentation& program, const ScopedData& data) const override;
};