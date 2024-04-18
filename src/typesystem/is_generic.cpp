
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

[[nodiscard]] bool CustomType::is_generic(const std::vector<std::string>& generics_names) const {
    if (generics_names.empty()) {
        return false;
    }
    for (const TypeSignature& generic : instanciated_generics){
        if (generic.is_generic(generics_names)) {
            return true;
        }
    }
    return generics_names.end() != std::find(
        generics_names.begin(), 
        generics_names.end(), 
        type_name
    );
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

[[nodiscard]] bool PrimitiveType::is_generic(const std::vector<std::string>& generics_names) const {
    return false;
}