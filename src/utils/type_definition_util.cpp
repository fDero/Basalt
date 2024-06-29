
#include "language/definitions.hpp"

size_t TypeDefinition::get_number_of_generics() const {
    if (std::holds_alternative<StructDefinition>(*this)) {
        return std::get<StructDefinition>(*this).template_generics_names.size();
    }
    if (std::holds_alternative<UnionDefinition>(*this)) {
        return std::get<UnionDefinition>(*this).template_generics_names.size();
    }
    if (std::holds_alternative<TypeAlias>(*this)) {
        return std::get<TypeAlias>(*this).template_generics_names.size();
    }
    throw;
}

std::string TypeDefinition::get_simple_name() const {
    if (std::holds_alternative<StructDefinition>(*this)) {
        return std::get<StructDefinition>(*this).struct_name;
    }
    if (std::holds_alternative<UnionDefinition>(*this)) {
        return std::get<UnionDefinition>(*this).union_name;
    }
    if (std::holds_alternative<TypeAlias>(*this)) {
        return std::get<TypeAlias>(*this).alias_name;
    }
    throw;
}