
#include "language/typesignatures.hpp"

[[nodiscard]] TypeSignatureBody::Kind TypeSignature::typesiganture_kind() const {
    return ptr->typesiganture_kind();
};

[[nodiscard]] TypeSignatureBody::Kind CustomType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::custom_type;
};

[[nodiscard]] TypeSignatureBody::Kind InlineUnion::typesiganture_kind() const {
    return TypeSignatureBody::Kind::inline_union;
};


[[nodiscard]] TypeSignatureBody::Kind TemplateType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::template_type;
};

[[nodiscard]] TypeSignatureBody::Kind PrimitiveType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::primitive_type;
};

[[nodiscard]] TypeSignatureBody::Kind PointerType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::pointer_type;
};

[[nodiscard]] TypeSignatureBody::Kind ArrayType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::array_type;
};

[[nodiscard]] TypeSignatureBody::Kind SliceType::typesiganture_kind() const {
    return TypeSignatureBody::Kind::slice_type;
};