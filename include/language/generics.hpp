
#pragma once
#include <map>
#include <string>
#include <vector>
#include "language/typesystem.hpp"
#include "language/definitions.hpp"
#include "misc/forward_declarations.hpp"

struct GenericSubstitutionRule {
    std::string to_be_replaced;
    TypeSignature replacement;
};

struct GenericSubstitutionRuleSet : public std::vector<GenericSubstitutionRule> {
    
    using Ref = std::shared_ptr<GenericSubstitutionRuleSet>;

    using vector<GenericSubstitutionRule>::vector;
    using vector<GenericSubstitutionRule>::operator=;
    using vector<GenericSubstitutionRule>::operator[];
    using vector<GenericSubstitutionRule>::push_back;
    
    [[nodiscard]] static GenericSubstitutionRuleSet zip_components_vectors (
        const std::vector<std::string>& template_generics,
        const std::vector<TypeSignature>& type_parameters
    );
};

class GenericsInstantiationEngine {

    public:
        GenericsInstantiationEngine(const GenericSubstitutionRuleSet& rules)
            : rules(rules) {}

        [[nodiscard]] TypeSignature instantiate_generic_typesignature(const TypeSignature& type_signature) const;
        [[nodiscard]] TypeSignature instantiate_template_type(const TemplateType& template_type) const;
        [[nodiscard]] CustomType instantiate_custom_type(const CustomType& type_signature) const;
        [[nodiscard]] PointerType instantiate_pointer_type(const PointerType& type_signature) const;
        [[nodiscard]] ArrayType instantiate_array_type(const ArrayType& type_signature) const;
        [[nodiscard]] SliceType instantiate_slice_type(const SliceType& type_signature) const;
        [[nodiscard]] InlineUnion instantiate_inline_union(const InlineUnion& type_signature) const;

        //[[nodiscard]] FunctionDefinition instantiate_generic_function_definition(const FunctionDefinition& function_definition) const;
        //[[nodiscard]] Statement instantiate_generic_statement(const Statement& statement) const;
        //[[nodiscard]] Expression instantiate_generic_expression(const Expression& expression) const;
        
        [[nodiscard]] TypeDefinition instantiate_generic_typedefinition(const TypeDefinition& type_signature) const;
        [[nodiscard]] UnionDefinition instantiate_generic_union(const UnionDefinition& type_signature) const;
        [[nodiscard]] StructDefinition instantiate_generic_struct(const StructDefinition& type_signature) const;
        [[nodiscard]] TypeAlias instantiate_generic_alias(const TypeAlias& type_signature) const;

    private:
        GenericSubstitutionRuleSet rules;
};