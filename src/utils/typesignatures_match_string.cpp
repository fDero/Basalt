
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

[[nodiscard]] std::string TypeSignature::to_match_string() const { 
    return ptr->to_match_string(); 
}

[[nodiscard]] std::string CustomType::to_match_string() const {
    assert_type_is_non_primitive(*this);
    if (instantiation_generics.empty()) {
        return type_name;
    }
    std::string match_string = type_name + "<";
    for (size_t i = 0; i < instantiation_generics.size(); i++){
        match_string += "?,";
    }
    match_string.back() = '>';
    return match_string;
}

[[nodiscard]] std::string PointerType::to_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}

[[nodiscard]] std::string ArrayType::to_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}

[[nodiscard]] std::string SliceType::to_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}

[[nodiscard]] std::string TemplateType::to_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}

[[nodiscard]] std::string PrimitiveType::to_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}