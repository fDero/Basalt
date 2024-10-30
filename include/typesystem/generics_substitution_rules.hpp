//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <map>
#include <string>
#include <vector>

#include "language/typesignatures.hpp"

struct GenericSubstitutionRule {

    std::string to_be_replaced;
    TypeSignature replacement;

    struct Set : public std::vector<GenericSubstitutionRule> {
    
        using Ref = std::shared_ptr<GenericSubstitutionRule::Set>;

        using vector<GenericSubstitutionRule>::vector;
        using vector<GenericSubstitutionRule>::operator=;
        using vector<GenericSubstitutionRule>::operator[];
        using vector<GenericSubstitutionRule>::push_back;

        [[nodiscard]] static GenericSubstitutionRule::Set zip_components_vectors (
            const std::vector<std::string>& template_generics,
            const std::vector<TypeSignature>& type_parameters
        );
    };
};