//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"

GenericSubstitutionRule::Set GenericSubstitutionRule::Set::zip_components_vectors (
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& type_parameters
) {
    assert_vectors_have_same_size_hence_they_can_be_zipped(template_generics, type_parameters);
    GenericSubstitutionRule::Set result;
    for (size_t i = 0; i < type_parameters.size(); i++) {
        const TypeSignature& instantiation_generic = type_parameters[i];
        const std::string& template_generic_name = template_generics[i];
        GenericSubstitutionRule rule = { template_generic_name, instantiation_generic};
        result.push_back(rule);
    }
    return result;
}