//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/typesignatures.hpp"

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