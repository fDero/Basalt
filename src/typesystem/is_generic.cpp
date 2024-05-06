
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

[[nodiscard]] bool BaseType::is_generic(const std::vector<std::string>& generics_names) const {
    for (const TypeSignature& generic : instantiationd_generics){
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