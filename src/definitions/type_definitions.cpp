
#include "language/definitions.hpp"

[[nodiscard]] std::string TypeDefinition::generate_id() const {
    if (std::holds_alternative<StructDefinition>(*this)){
        return std::get<StructDefinition>(*this).generate_struct_id();
    }
    if (std::holds_alternative<UnionDefinition>(*this)){
        return std::get<UnionDefinition>(*this).generate_union_id();
    }
    if (std::holds_alternative<TypeAlias>(*this)){
        return std::get<TypeAlias>(*this).generate_alias_id();
    }
    throw;
}
    
[[nodiscard]] std::string TypeDefinition::generate_match_pattern() const {
    if (std::holds_alternative<StructDefinition>(*this)){
        return std::get<StructDefinition>(*this).generate_match_pattern();
    }
    if (std::holds_alternative<UnionDefinition>(*this)){
        return std::get<UnionDefinition>(*this).generate_match_pattern();
    }
    if (std::holds_alternative<TypeAlias>(*this)){
        return std::get<TypeAlias>(*this).generate_match_pattern();
    }
    throw;
}
    
void TypeDefinition::instantiate_generics(const BaseType& concrete_type){
    if (std::holds_alternative<StructDefinition>(*this)){
        return std::get<StructDefinition>(*this).instantiate_generics(concrete_type);
    }
    if (std::holds_alternative<UnionDefinition>(*this)){
        return std::get<UnionDefinition>(*this).instantiate_generics(concrete_type);
    }
    if (std::holds_alternative<TypeAlias>(*this)){
        return std::get<TypeAlias>(*this).instantiate_generics(concrete_type);
    }
    throw;
}