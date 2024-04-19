
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>


void StructDefinition::instanciate_generics(const CustomType& concrete_type) {
    // ASSERT CONCRETE-TYPE-NAME IS SOMEWHAT COMPATIBLE WITH STRUCT-NAME
    const std::vector<TypeSignature>& instanciated_generics = concrete_type.instanciated_generics;
    const std::vector<std::string>& template_generics = template_generics_names;
    struct_name = concrete_type.to_string();
    template_generics_names.clear();
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, instanciated_generics
    );
    for (StructDefinition::Field& concrete_field : fields){
        concrete_field.field_type.instanciate_generics(generic_substitution_rules);
    }
}

void UnionDefinition::instanciate_generics(const CustomType& concrete_type) {
    // ASSERT CONCRETE-TYPE-NAME IS SOMEWHAT COMPATIBLE WITH UNION-NAME
    const std::vector<TypeSignature>& instanciated_generics = concrete_type.instanciated_generics;
    const std::vector<std::string>& template_generics = template_generics_names;
    union_name = concrete_type.to_string();
    template_generics_names.clear();
    auto generic_substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
        template_generics, instanciated_generics
    );
    for (TypeSignature& alternative : types){
        alternative.instanciate_generics(generic_substitution_rules);
    }
}

[[nodiscard]] GenericSubstitutionRuleSet GenericSubstitutionRuleSet::zip_components_vectors (
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& instanciated_generics
){
    assert_vectors_have_same_size_hence_they_can_be_zipped(template_generics, instanciated_generics);
    GenericSubstitutionRuleSet result;
    for (size_t i = 0; i < instanciated_generics.size(); i++){
        const TypeSignature& instanciated_generic = instanciated_generics[i];
        const std::string& template_generic_name = template_generics[i];
        TypeSignature template_generic = CustomType{ template_generic_name, {} };
        GenericSubstitutionRule rule = { template_generic, instanciated_generic};
        result.push_back(rule);
    }
    return result;
}

void TypeSignature::instanciate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    for (GenericSubstitutionRule rule : generic_substitution_rules){
        if (this->to_string() == rule.to_be_substituded.to_string()){
            *this = rule.replacement;
            return;
        }
    }
}

void CustomType::instanciate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    for (TypeSignature& generic : instanciated_generics){
        generic.instanciate_generics(generic_substitution_rules);
    }
}

void PointerType::instanciate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    pointed_type.instanciate_generics(generic_substitution_rules);
}

void SliceType::instanciate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    stored_type.instanciate_generics(generic_substitution_rules);
}

void ArrayType::instanciate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    stored_type.instanciate_generics(generic_substitution_rules);
}

void PrimitiveType::instanciate_generics(const GenericSubstitutionRuleSet& generic_substitution_rules) {
    // Do nothing, primitive types cannot be generic
}

