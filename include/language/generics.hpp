
#pragma once
#include <map>
#include <string>
#include <vector>
#include "language/typesystem.hpp"
#include "language/definitions.hpp"
#include "misc/aliases.hpp"

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
        const TemplateGenerics& template_generics,
        const ConcreteGenerics& instantiation_generics
    );
};