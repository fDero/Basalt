
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

void StructDefinition::instantiate_generics(const CustomType& concrete_type) {
    assert_instantiation_struct_is_compatible_with_template_struct(concrete_type, *this);
    const std::vector<TypeSignature>& type_parameters = concrete_type.type_parameters;
    const std::vector<std::string>& template_generics = template_generics_names;
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, type_parameters
    );
    template_generics_names.clear();
    for (StructDefinition::Field& concrete_field : fields) {
        concrete_field.field_type.instantiate_generics(generic_substitution_rules);
    }
}

void UnionDefinition::instantiate_generics(const CustomType& concrete_type) {
    assert_instantiation_union_is_compatible_with_template_union(concrete_type, *this);
    const std::vector<TypeSignature>& type_parameters = concrete_type.type_parameters;
    const std::vector<std::string>& template_generics = template_generics_names;
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, type_parameters
    );
    template_generics_names.clear();
    for (TypeSignature& alternative : types) {
        alternative.instantiate_generics(generic_substitution_rules);
    }
}

void TypeAlias::instantiate_generics(const CustomType& concrete_type) {
    assert_instantiation_union_is_compatible_with_template_alias(concrete_type, *this);
    const std::vector<TypeSignature>& type_parameters = concrete_type.type_parameters;
    const std::vector<std::string>& template_generics = template_generics_names;
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, type_parameters
    );
    template_generics_names.clear();
    aliased_type.instantiate_generics(generic_substitution_rules);
}

void TypeDefinition::instantiate_generics(const CustomType& concrete_type) {
    if (std::holds_alternative<StructDefinition>(*this)) {
        std::get<StructDefinition>(*this).instantiate_generics(concrete_type);
    }
    else if (std::holds_alternative<UnionDefinition>(*this)) {
        std::get<UnionDefinition>(*this).instantiate_generics(concrete_type);
    }
    else if (std::holds_alternative<TypeAlias>(*this)) {
        std::get<TypeAlias>(*this).instantiate_generics(concrete_type);
    }
    else {
        assert_unreachable();
    }
}

void TypeDefinition::set_name(const std::string& name) {
    if (std::holds_alternative<StructDefinition>(*this)) {
        std::get<StructDefinition>(*this).struct_name = name;
    }
    else if (std::holds_alternative<UnionDefinition>(*this)) {
        std::get<UnionDefinition>(*this).union_name = name;
    }
    else if (std::holds_alternative<TypeAlias>(*this)) {
        std::get<TypeAlias>(*this).alias_name = name;
    }
    else {
        assert_unreachable();
    }
}

[[nodiscard]] GenericSubstitutionRuleSet GenericSubstitutionRuleSet::zip_components_vectors (
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& type_parameters
) {
    assert_vectors_have_same_size_hence_they_can_be_zipped(template_generics, type_parameters);
    GenericSubstitutionRuleSet result;
    for (size_t i = 0; i < type_parameters.size(); i++) {
        const TypeSignature& instantiation_generic = type_parameters[i];
        const std::string& template_generic_name = template_generics[i];
        GenericSubstitutionRule rule = { template_generic_name, instantiation_generic};
        result.push_back(rule);
    }
    return result;
}

void TypeSignature::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    for (GenericSubstitutionRule rule : generic_substitution_rules) {
        if (this->is<TemplateType>() && this->get<TemplateType>().type_name == rule.to_be_substituded) {
            *this = rule.replacement;
            return;
        }
    }
    ptr->instantiate_generics(generic_substitution_rules);
}

void CustomType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    for (TypeSignature& generic : type_parameters) {
        generic.instantiate_generics(generic_substitution_rules);
    }
}

void PointerType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    pointed_type.instantiate_generics(generic_substitution_rules);
}

void SliceType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    stored_type.instantiate_generics(generic_substitution_rules);
}

void ArrayType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    stored_type.instantiate_generics(generic_substitution_rules);
}

void TemplateType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {}

void PrimitiveType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {}
