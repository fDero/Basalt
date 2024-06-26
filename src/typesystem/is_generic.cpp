
#include "language/generics.hpp"
#include <regex>

[[nodiscard]] bool TypeSignature::is_generic(const std::vector<std::string>& generic_names) const {
    return ptr->is_generic(generic_names);
}

[[nodiscard]] bool CustomType::is_generic(const std::vector<std::string>& generics_names) const {
    for (const TypeSignature& generic : type_parameters) {
        if (generic.is_generic(generics_names)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] bool PointerType::is_generic(const std::vector<std::string>& generics_names) const {
    return !generics_names.empty() && pointed_type.is_generic(generics_names);
}

[[nodiscard]] bool ArrayType::is_generic(const std::vector<std::string>& generics_names) const {
    return !generics_names.empty() && stored_type.is_generic(generics_names);
}

[[nodiscard]] bool SliceType::is_generic(const std::vector<std::string>& generics_names) const {
    return !generics_names.empty() && stored_type.is_generic(generics_names);
}

[[nodiscard]] bool TemplateType::is_generic(const std::vector<std::string>& generics_names) const {
    return true;
}

[[nodiscard]] bool PrimitiveType::is_generic(const std::vector<std::string>& generics_names) const {
    return false;
}
