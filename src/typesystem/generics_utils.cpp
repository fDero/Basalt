
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"

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