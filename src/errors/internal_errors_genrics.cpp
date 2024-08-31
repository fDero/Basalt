
#include <vector>

#include "errors/internal_errors.hpp"
#include "language/generics.hpp"

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& type_parameters
) {
    #ifdef DEBUG_BUILD
    if (template_generics.size() != type_parameters.size()) {
        throw InternalError{ "template generics and instantiation generics have different sizes" };
    }
    #endif
}