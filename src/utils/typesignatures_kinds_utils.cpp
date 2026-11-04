//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cstddef>
#include <string>
#include <vector>

#include "language/typesignatures.hpp"
#include "misc/debug_informations_aware_entity.hpp"


TypeSignatureBody::Kind TypeSignature::typesiganture_kind() const {
    return ptr->typesiganture_kind();
};

TypeSignatureBody::Kind CustomType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::custom_type;
};

TypeSignatureBody::Kind InlineUnion::typesiganture_kind() const {
    return TypeSignatureBody::Kind::inline_union;
};

TypeSignatureBody::Kind TemplateType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::template_type;
};

TypeSignatureBody::Kind PrimitiveType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::primitive_type;
};

TypeSignatureBody::Kind PointerType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::pointer_type;
};

TypeSignatureBody::Kind ArrayType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::array_type;
};

TypeSignatureBody::Kind SliceType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::slice_type;
};
const DebugInformationsAwareEntity& TypeSignature::as_debug_informations_aware_entity() const {
    return ptr->as_debug_informations_aware_entity();
}

static std::string type_parameters_to_source_string(const std::vector<TypeSignature>& type_parameters) {
    if (type_parameters.empty()) {
        return "";
    }
    std::string result = "<";
    for (size_t i = 0; i < type_parameters.size(); i++) {
        result += (i == 0 ? "" : ", ") + type_parameters[i].to_source_string();
    }
    return result + ">";
}

std::string TypeSignature::to_source_string() const {
    switch (typesiganture_kind()) {
        case TypeSignatureBody::Kind::primitive_type: return get<PrimitiveType>().type_name;
        case TypeSignatureBody::Kind::template_type:  return get<TemplateType>().type_name;
        case TypeSignatureBody::Kind::pointer_type:   return "#" + get<PointerType>().pointed_type.to_source_string();
        case TypeSignatureBody::Kind::slice_type:     return "$" + get<SliceType>().stored_type.to_source_string();
        case TypeSignatureBody::Kind::array_type: {
            const ArrayType& array_type = get<ArrayType>();
            return "[" + std::to_string(array_type.array_length) + "]" + array_type.stored_type.to_source_string();
        }
        case TypeSignatureBody::Kind::custom_type: {
            const CustomType& custom_type = get<CustomType>();
            std::string prefix = custom_type.package_prefix.empty() ? "" : custom_type.package_prefix + "::";
            return prefix + custom_type.type_name + type_parameters_to_source_string(custom_type.type_parameters);
        }
        case TypeSignatureBody::Kind::inline_union: {
            std::string result;
            for (const TypeSignature& alternative : get<InlineUnion>().alternatives) {
                result += (result.empty() ? "" : " | ") + alternative.to_source_string();
            }
            return result;
        }
    }
    return "<unknown type>";
}
