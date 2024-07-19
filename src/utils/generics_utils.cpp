
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"

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

[[nodiscard]] const std::vector<std::string>& TypeDefinition::get_template_generics() const {
    if (std::holds_alternative<StructDefinition>(*this)) {
        return std::get<StructDefinition>(*this).template_generics_names;
    }
    else if (std::holds_alternative<UnionDefinition>(*this)) {
        return std::get<UnionDefinition>(*this).template_generics_names;
    }
    else if (std::holds_alternative<TypeAlias>(*this)) {
        return std::get<TypeAlias>(*this).template_generics_names;
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