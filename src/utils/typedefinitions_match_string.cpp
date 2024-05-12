
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"

[[nodiscard]] std::string TypeAlias::generate_match_pattern() const {
    std::string pattern_tag_name = alias_name;
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

[[nodiscard]] std::string UnionDefinition::generate_match_pattern() const {
    std::string pattern_tag_name = union_name;
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