//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>
#include <vector>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/expressions.hpp"
#include "language/typesignatures.hpp"


void ensure_array_literal_contains_the_right_number_of_elements(
    const ArrayLiteral& array_literal
) {
    bool has_size = array_literal.array_length.has_value();
    if (has_size && array_literal.elements.size() != *array_literal.array_length) {
        throw ValidationError(
            "array literal declares " + std::to_string(*array_literal.array_length) +
            " elements, but contains " + std::to_string(array_literal.elements.size()),
            array_literal.as_debug_informations_aware_entity()
        );
    }
}

void ensure_element_is_compatible_with_array_literal(
    bool element_can_be_analyzed,
    bool element_is_compatible,
    const ArrayLiteral& array_literal,
    const Expression& element,
    const std::optional<TypeSignature>& element_type
) {
    if (!element_can_be_analyzed || element_is_compatible) {
        return;
    }
    throw ValidationError(
        "array element has type " + describe_type(element_type) +
        ", but the array holds elements of type " + describe_type(array_literal.stored_type),
        element.as_debug_informations_aware_entity()
    );
}
