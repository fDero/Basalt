
#include "language/generics.hpp"
#include <regex>

[[nodiscard]] bool TypeSignature::is_generic() const {
    return ptr->is_generic();
}

[[nodiscard]] bool CustomType::is_generic() const {
    for (const TypeSignature& generic : type_parameters) {
        if (generic.is_generic()) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] bool InlineUnion::is_generic() const {
    for (const TypeSignature& alternative : alternatives) {
        if (alternative.is_generic()) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] bool PointerType::is_generic() const {
    return pointed_type.is_generic();
}

[[nodiscard]] bool ArrayType::is_generic() const {
    return stored_type.is_generic();
}

[[nodiscard]] bool SliceType::is_generic() const {
    return stored_type.is_generic();
}

[[nodiscard]] bool TemplateType::is_generic() const {
    return true;
}

[[nodiscard]] bool PrimitiveType::is_generic() const {
    return false;
}
