
#pragma once

#include "typesystem/generics_substitution_rules.hpp"
#include "language/typesignatures.hpp"
#include "language/functions.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "misc/forward_declarations.hpp"

class GenericsInstantiationEngine {

    public:
        GenericsInstantiationEngine(const GenericSubstitutionRule::Set& rules)
            : rules(rules) {}

        [[nodiscard]] TypeSignature instantiate_generic_typesignature(const TypeSignature& type_signature) const;
        [[nodiscard]] TypeSignature instantiate_template_type(const TemplateType& template_type) const;
        [[nodiscard]] CustomType instantiate_custom_type(const CustomType& type_signature) const;
        [[nodiscard]] PointerType instantiate_pointer_type(const PointerType& type_signature) const;
        [[nodiscard]] ArrayType instantiate_array_type(const ArrayType& type_signature) const;
        [[nodiscard]] SliceType instantiate_slice_type(const SliceType& type_signature) const;
        [[nodiscard]] InlineUnion instantiate_inline_union(const InlineUnion& type_signature) const;
        
        [[nodiscard]] Statement instantiate_generic_statement(const Statement& statement) const;
        [[nodiscard]] VariableDeclaration instantiate_generic_variable_declaration(const VariableDeclaration& statement) const;
        [[nodiscard]] ConstDeclaration instantiate_generic_const_declaration(const ConstDeclaration& statement) const;
        [[nodiscard]] Assignment instantiate_generic_assignment(const Assignment& statement) const;
        [[nodiscard]] Conditional instantiate_generic_conditional(const Conditional& statement) const;
        [[nodiscard]] WhileLoop instantiate_generic_while_loop(const WhileLoop& statement) const;
        [[nodiscard]] UntilLoop instantiate_generic_until_loop(const UntilLoop& statement) const;
        [[nodiscard]] Return instantiate_generic_return_statement(const Return& statement) const;
        
        [[nodiscard]] Expression instantiate_generic_expression(const Expression& expression) const;
        [[nodiscard]] FunctionCall instantiate_generic_function_call(const FunctionCall& expression) const;
        [[nodiscard]] BinaryOperator instantiate_generic_binary_operator(const BinaryOperator& expression) const;
        [[nodiscard]] UnaryOperator instantiate_generic_unary_operator(const UnaryOperator& expression) const;
        [[nodiscard]] ArrayLiteral instantiate_generic_array_literal(const ArrayLiteral& expression) const;
        [[nodiscard]] TypeOperator instantiate_generic_type_operator(const TypeOperator& expression) const;
        [[nodiscard]] DotMemberAccess instantiate_generic_dot_member_access(const DotMemberAccess& expression) const;
        [[nodiscard]] SquareBracketsAccess instantiate_generic_square_bracket_access(const SquareBracketsAccess& expression) const;

        [[nodiscard]] TypeDefinition instantiate_generic_typedefinition(const TypeDefinition& type_signature) const;
        [[nodiscard]] UnionDefinition instantiate_generic_union(const UnionDefinition& type_signature) const;
        [[nodiscard]] StructDefinition instantiate_generic_struct(const StructDefinition& type_signature) const;
        [[nodiscard]] TypeAlias instantiate_generic_alias(const TypeAlias& type_signature) const;
        [[nodiscard]] FunctionDefinition::Ref instantiate_generic_function(const FunctionDefinition& function_definition) const;

    private:
        GenericSubstitutionRule::Set rules;
};