
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

[[nodiscard]] std::string StructDefinition::generate_struct_id() const {
    return filename + "/" + std::to_string(line_number) + "/" +
        std::to_string(char_pos) + "/" + std::to_string(tok_num) + "/" + struct_name;
}

[[nodiscard]] std::string StructDefinition::generate_match_pattern() const {
    std::string pattern_tag_name = struct_name;
    int number_of_generics = template_generics_names.size();
    if (number_of_generics > 0) {
        pattern_tag_name += "<";
        for (int i = 0; i < number_of_generics; i++) { 
            pattern_tag_name += "?,";
        }
        pattern_tag_name.back() = '>';
    }
    return pattern_tag_name;
}

[[nodiscard]] std::string BaseType::struct_retrieval_match_string() const {
    assert_type_is_non_primitive(*this);
    if (instanciated_generics.empty()) {
        return type_name;
    }
    std::string match_string = type_name + "<";
    for (size_t i = 0; i < instanciated_generics.size(); i++){
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