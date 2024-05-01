
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>
#include "language/syntax.hpp"

[[nodiscard]] bool BaseType::is_core_language_type() const {
    return primitive_types.find(type_name) != primitive_types.end();
}

[[nodiscard]] bool PointerType::is_core_language_type() const {
    return true;
}

[[nodiscard]] bool ArrayType::is_core_language_type() const {
    return true;
}

[[nodiscard]] bool SliceType::is_core_language_type() const {
    return true;
}
