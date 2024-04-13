
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

std::optional<GenericSubstitutionRuleSet> combine_generic_substitution_rulesets(
    const std::optional<GenericSubstitutionRuleSet>& lx,
    const std::optional<GenericSubstitutionRuleSet>& rx
){
    if (!(lx.has_value() && rx.has_value())) return std::nullopt;
    GenericSubstitutionRuleSet result = *lx;
    result.insert(result.end(), rx->begin(), rx->end());
    return result;
}

std::optional<GenericSubstitutionRuleSet> apply_generics_to_typesignature_vectors(
    const std::vector<TypeSignature>& template_types,
    const std::vector<TypeSignature>& concrete_types,
    const std::vector<std::string>& generic_names
){
    std::optional<GenericSubstitutionRuleSet> result = GenericSubstitutionRuleSet{};
    for (int i = 0; i < template_types.size(); i++){
        if (!result.has_value()) break;
        std::optional<GenericSubstitutionRuleSet> other = apply_generics_to_typesignature(
            template_types[i], concrete_types[i], generic_names
        );
        result = combine_generic_substitution_rulesets(result, other);
    }
    return result;
}

bool current_typesignature_is_target_generic(
    const TypeSignature& template_type,
    const std::vector<std::string>& generic_names
){
    assert_typesignature_is<BaseType>(template_type);
    std::string base_type = template_type.get<BaseType>().type_name;
    auto search_outcome = std::find(generic_names.begin(), generic_names.end(), base_type);
    return (search_outcome != generic_names.end());
}

bool current_typesignature_is_incompatible_with_instanciation(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type
){
    assert_typesignature_is<BaseType>(template_type);
    assert_typesignature_is<BaseType>(concrete_type);
    std::string concrete_type_name = concrete_type.get<BaseType>().type_name;
    std::string template_type_name = template_type.get<BaseType>().type_name;
    int concrete_type_generics_size = concrete_type.get<BaseType>().instanciated_generics.size();
    int template_type_generics_size = template_type.get<BaseType>().instanciated_generics.size();
    return 
        !(concrete_type_name == template_type_name) || 
        !(concrete_type_generics_size == template_type_generics_size);
}

std::optional<GenericSubstitutionRuleSet> apply_generics_to_base_type(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
){
    assert_typesignature_is<BaseType>(template_type);
    if (current_typesignature_is_target_generic(template_type, generic_names)){
        assert_basetype_has_no_generics(template_type);
        GenericSubstitutionRuleSet result;
        result.push_back(GenericSubstitutionRule { template_type, concrete_type });
        return result;
    }
    else if (concrete_type.is<BaseType>()){
        auto template_generic_vec = template_type.get<BaseType>().instanciated_generics;
        auto concrete_generic_vec = concrete_type.get<BaseType>().instanciated_generics;
        if (current_typesignature_is_incompatible_with_instanciation(template_type, concrete_type)) return std::nullopt;
        else return apply_generics_to_typesignature_vectors(template_generic_vec, concrete_generic_vec, generic_names);
    }
    return std::nullopt;
}