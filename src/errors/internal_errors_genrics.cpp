
#include "errors/internal_errors.hpp"
#include "toolchain/rappresentation.hpp"
#include "language/generics.hpp"
#include <vector>

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& instanciated_generics
){
    #ifdef DEBUG_BUILD
    if (template_generics.size() != instanciated_generics.size()){
        throw InternalError{ "template generics and instanciated generics have different sizes" };
    }
    #endif
}

void assert_instanciated_struct_is_compatible_with_template_struct(
    const CustomType& concrete_type,
    const StructDefinition& template_struct
){
    #ifdef DEBUG_BUILD
    if (concrete_type.instanciated_generics.size() != template_struct.template_generics_names.size()){
        throw InternalError{ "concrete type and template struct have different number of generics" };
    }
    if (concrete_type.to_match_string() != template_struct.generate_match_pattern()){
        throw InternalError{ "concrete type and template struct have a different match pattern" };
    }
    #endif
}

void assert_instanciated_union_is_compatible_with_template_union(
    const CustomType& concrete_type,
    const UnionDefinition& template_union
){
    #ifdef DEBUG_BUILD
    if (concrete_type.instanciated_generics.size() != template_union.template_generics_names.size()){
        throw InternalError{ "concrete type and template union have different number of generics" };
    }
    if (concrete_type.to_match_string() != template_union.generate_match_pattern()){
        throw InternalError{ "concrete type and template union have a different match pattern" };
    }
    #endif
}