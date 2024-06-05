
#include "errors/parsing_errors.hpp"

void ensure_primitive_type_is_not_generic(
    const Token& typesignature_token,
    const std::vector<TypeSignature>& template_generics
) {
    if (!template_generics.empty()) {
        throw ParsingError {
            "primitive types cannot be generic",
            typesignature_token
        };
    }
}