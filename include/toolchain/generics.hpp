
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