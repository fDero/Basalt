//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"

bool TypeSignature::is_generic() const {
    return ptr->is_generic();
}

bool CustomType::is_generic() const {
    for (const TypeSignature& generic : type_parameters) {
        if (generic.is_generic()) {
            return true;
        }
    }
    return false;
}

bool InlineUnion::is_generic() const {
    for (const TypeSignature& alternative : alternatives) {
        if (alternative.is_generic()) {
            return true;
        }
    }
    return false;
}

bool PointerType::is_generic() const {
    return pointed_type.is_generic();
}

bool ArrayType::is_generic() const {
    return stored_type.is_generic();
}

bool SliceType::is_generic() const {
    return stored_type.is_generic();
}

bool PrimitiveType::is_generic() const {
    return false;
}

bool TemplateType::is_generic() const {
    return true;
}