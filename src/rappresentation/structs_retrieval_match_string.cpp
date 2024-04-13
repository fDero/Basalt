
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

[[nodiscard]] std::string BaseType::struct_retrieval_match_string() const {
    assert_type_is_non_primitive(*this);
    std::string fully_qualified_name = type_name;
    if (instanciated_generics.empty()) {
        return fully_qualified_name;
    }
    std::string match_string = fully_qualified_name + "<";
    for (int i = 0; i < instanciated_generics.size(); i++){
        match_string += "?,";
    }
    match_string.back() = '>';
    return match_string;
}

[[nodiscard]] std::string PointerType::struct_retrieval_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}

[[nodiscard]] std::string ArrayType::struct_retrieval_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}

[[nodiscard]] std::string SliceType::struct_retrieval_match_string() const {
    throw_attempt_to_retrieve_struct_definition_from_primitive_type(*this);
}