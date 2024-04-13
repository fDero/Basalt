
#pragma once
#include <map>
#include <string>
#include <vector>
#include "toolchain/parser.hpp"

struct GenericSubstitutionRule {
    TypeSignature to_be_substituded;
    TypeSignature replacement;
};

struct GenericSubstitutionRuleSet : public std::vector<GenericSubstitutionRule> {
    
    using vector<GenericSubstitutionRule>::vector;
    using vector<GenericSubstitutionRule>::operator=;
    using vector<GenericSubstitutionRule>::operator[];
    using vector<GenericSubstitutionRule>::push_back;
    
    
    [[nodiscard]] static GenericSubstitutionRuleSet zip_components_vectors (
        const std::vector<std::string>& template_generics,
        const std::vector<TypeSignature>& instanciated_generics
    ){
        // ASSERT SAME SIZE
        GenericSubstitutionRuleSet result;
        for (size_t i = 0; i < instanciated_generics.size(); i++){
            const TypeSignature& instanciated_generic = instanciated_generics[i];
            const std::string& template_generic_name = template_generics[i];
            TypeSignature template_generic = BaseType{ template_generic_name, {} };
            GenericSubstitutionRule rule = { template_generic, instanciated_generic};
            result.push_back(rule);
        }
        return result;
    }
};




[[nodiscard]] std::optional<GenericSubstitutionRuleSet> apply_generics_to_typesignature(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
);

[[nodiscard]] std::optional<GenericSubstitutionRuleSet> apply_generics_to_base_type(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
);