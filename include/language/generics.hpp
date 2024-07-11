
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