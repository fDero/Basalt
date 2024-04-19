
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>
#include "language/syntax.hpp"

[[nodiscard]] bool CustomType::is_core_language_type() const {
    return false;
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

[[nodiscard]] bool PrimitiveType::is_core_language_type() const {
    return true;
}
