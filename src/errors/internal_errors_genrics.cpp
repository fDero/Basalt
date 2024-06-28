
#include "errors/internal_errors.hpp"
#include "toolchain/representation.hpp"
#include "language/generics.hpp"
#include <vector>

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& type_parameters
) {
    #ifdef DEBUG_BUILD
    if (template_generics.size() != type_parameters.size()) {
        throw InternalError{ "template generics and instantiation generics have different sizes" };
    }
    #endif
}

void assert_instantiation_struct_is_compatible_with_template_struct(
    const CustomType& concrete_type,
    const StructDefinition& template_struct
) {
    #ifdef DEBUG_BUILD
    if (concrete_type.type_parameters.size() != template_struct.template_generics_names.size()) {
        throw InternalError{ "concrete type and template struct have different number of generics" };
    }
    #endif
}

void assert_instantiation_union_is_compatible_with_template_union(
    const CustomType& concrete_type,
    const UnionDefinition& template_union
) {
    #ifdef DEBUG_BUILD
    if (concrete_type.type_parameters.size() != template_union.template_generics_names.size()) {
        throw InternalError{ "concrete type and template union have different number of generics" };
    }
    #endif
}

void assert_instantiation_union_is_compatible_with_template_alias(
    const CustomType& concrete_type,
    const TypeAlias& template_alias
) {
    #ifdef DEBUG_BUILD
    if (concrete_type.type_parameters.size() != template_alias.template_generics_names.size()) {
        throw InternalError{ "concrete type and template union have different number of generics" };
    }
    #endif
}