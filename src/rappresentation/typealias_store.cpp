
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"

void TypeDefinitionsRegister::store_type_alias_definition(const TypeAlias& alias_def){
    const std::string match_pattern = alias_def.generate_match_pattern();
    ensure_alias_doesnt_already_exists(match_pattern, alias_def, type_definitions);
    type_definitions.insert(std::make_pair(match_pattern, alias_def));
}

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

[[nodiscard]] std::string TypeAlias::generate_alias_id() const {
    return filename + "/" + std::to_string(line_number) + "/" +
        std::to_string(char_pos) + "/" + std::to_string(tok_number) + "/" + alias_name;
}