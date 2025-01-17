//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/generics_substitution_rules.hpp"
#include "language/typesignatures.hpp"
#include "language/functions.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "misc/forward_declarations.hpp"

class GenericsInstantiationEngine {

    public:
        GenericsInstantiationEngine(const GenericSubstitutionRule::Set& rules)
            : rules(rules) {}

        [[nodiscard]] TypeSignature instantiate_generic_typesignature(const TypeSignature& type_signature);
        [[nodiscard]] TypeSignature instantiate_template_type(const TemplateType& template_type);
        [[nodiscard]] CustomType instantiate_custom_type(const CustomType& type_signature);
        [[nodiscard]] PointerType instantiate_pointer_type(const PointerType& type_signature);
        [[nodiscard]] ArrayType instantiate_array_type(const ArrayType& type_signature);
        [[nodiscard]] SliceType instantiate_slice_type(const SliceType& type_signature);
        [[nodiscard]] InlineUnion instantiate_inline_union(const InlineUnion& type_signature);
        
        [[nodiscard]] Statement instantiate_generic_statement(const Statement& statement);
        [[nodiscard]] VariableDeclaration instantiate_generic_variable_declaration(const VariableDeclaration& statement);
        [[nodiscard]] ConstDeclaration instantiate_generic_const_declaration(const ConstDeclaration& statement);
        [[nodiscard]] Assignment instantiate_generic_assignment(const Assignment& statement);
        [[nodiscard]] Conditional instantiate_generic_conditional(const Conditional& statement);
        [[nodiscard]] WhileLoop instantiate_generic_while_loop(const WhileLoop& statement);
        [[nodiscard]] UntilLoop instantiate_generic_until_loop(const UntilLoop& statement);
        [[nodiscard]] Return instantiate_generic_return_statement(const Return& statement);
        
        [[nodiscard]] Expression instantiate_generic_expression(const Expression& expression);
        [[nodiscard]] FunctionCall instantiate_generic_function_call(const FunctionCall& expression);
        [[nodiscard]] BinaryOperator instantiate_generic_binary_operator(const BinaryOperator& expression);
        [[nodiscard]] UnaryOperator instantiate_generic_unary_operator(const UnaryOperator& expression);
        [[nodiscard]] ArrayLiteral instantiate_generic_array_literal(const ArrayLiteral& expression);
        [[nodiscard]] TypeOperator instantiate_generic_type_operator(const TypeOperator& expression);
        [[nodiscard]] DotMemberAccess instantiate_generic_dot_member_access(const DotMemberAccess& expression);
        [[nodiscard]] SquareBracketsAccess instantiate_generic_square_bracket_access(const SquareBracketsAccess& expression);

        [[nodiscard]] TypeDefinition instantiate_generic_typedefinition(
            const TypeDefinition& type_signature,
            const std::string& new_type_name
        );

        [[nodiscard]] UnionDefinition instantiate_generic_union(
            const UnionDefinition& type_signature,
            const std::string& new_union_name
        );
        
        [[nodiscard]] StructDefinition instantiate_generic_struct(
            const StructDefinition& type_signature,
            const std::string& new_struct_name
        );
        
        [[nodiscard]] TypeAlias instantiate_generic_alias(
            const TypeAlias& type_signature,
            const std::string& new_alias_name
        );
        
        [[nodiscard]] FunctionDefinition::Ref instantiate_generic_function(
            const FunctionDefinition& function_definition,
            const std::string& new_function_name
        );

    private:
        GenericSubstitutionRule::Set rules;
};