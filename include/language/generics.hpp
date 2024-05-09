
#pragma once
#include <map>
#include <string>
#include <vector>
#include "language/typesystem.hpp"

struct GenericSubstitutionRule {
    std::string to_be_substituded;
    TypeSignature replacement;
};

struct GenericSubstitutionRuleSet : public std::vector<GenericSubstitutionRule> {
    
    using vector<GenericSubstitutionRule>::vector;
    using vector<GenericSubstitutionRule>::operator=;
    using vector<GenericSubstitutionRule>::operator[];
    using vector<GenericSubstitutionRule>::push_back;
    
    [[nodiscard]] static GenericSubstitutionRuleSet zip_components_vectors (
        const std::vector<std::string>& template_generics,
        const std::vector<TypeSignature>& instantiationd_generics
    );
};