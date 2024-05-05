
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>


void StructDefinition::instantiate_generics(const BaseType& concrete_type) {
    
    std::cerr << concrete_type.instantiationd_generics.size() << " <-- " << std::endl;
    std::cerr << template_generics_names.size() << " <-- " << std::endl;

    assert_instantiationd_struct_is_compatible_with_template_struct(concrete_type, *this);
    const std::vector<TypeSignature>& instantiationd_generics = concrete_type.instantiationd_generics;
    const std::vector<std::string>& template_generics = template_generics_names;
    struct_name = concrete_type.to_string();
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, instantiationd_generics
    );
    template_generics_names.clear();
    for (StructDefinition::Field& concrete_field : fields){
        concrete_field.field_type.instantiate_generics(generic_substitution_rules);
    }
}

void UnionDefinition::instantiate_generics(const BaseType& concrete_type) {
    assert_instantiationd_union_is_compatible_with_template_union(concrete_type, *this);
    const std::vector<TypeSignature>& instantiationd_generics = concrete_type.instantiationd_generics;
    const std::vector<std::string>& template_generics = template_generics_names;
    union_name = concrete_type.to_string();
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, instantiationd_generics
    );
    template_generics_names.clear();
    for (TypeSignature& alternative : types){
        alternative.instantiate_generics(generic_substitution_rules);
    }
}

[[nodiscard]] GenericSubstitutionRuleSet GenericSubstitutionRuleSet::zip_components_vectors (
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& instantiationd_generics
){
    assert_vectors_have_same_size_hence_they_can_be_zipped(template_generics, instantiationd_generics);
    GenericSubstitutionRuleSet result;
    for (size_t i = 0; i < instantiationd_generics.size(); i++){
        const TypeSignature& instantiationd_generic = instantiationd_generics[i];
        const std::string& template_generic_name = template_generics[i];
        GenericSubstitutionRule rule = { template_generic_name, instantiationd_generic};
        result.push_back(rule);
    }
    return result;
}

void TypeSignature::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    for (GenericSubstitutionRule rule : generic_substitution_rules){
        if (this->to_string() == rule.to_be_substituded){
            *this = rule.replacement;
            return;
        }
    }
    ptr->instantiate_generics(generic_substitution_rules);
}

void BaseType::instantiate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    for (TypeSignature& generic : instantiationd_generics){
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

