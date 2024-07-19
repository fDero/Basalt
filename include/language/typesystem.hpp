
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/syntax.hpp"
#include "misc/forward_declarations.hpp"

struct TypeSignatureBody : public DebugInformationsAwareEntity {

    TypeSignatureBody(const DebugInformationsAwareEntity& token)
        : DebugInformationsAwareEntity(token) {}

    enum class Kind {
        custom_type,
        inline_union,
        template_type,
        primitive_type,
        pointer_type,
        array_type,
        slice_type
    };

    [[nodiscard]] virtual Kind typesiganture_kind() const = 0;
    [[nodiscard]] virtual bool is_generic() const = 0;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return Polymorph::ptr->typesiganture_kind();
    };

    [[nodiscard]] bool is_generic() const;
};

struct CustomType : public TypeSignatureBody {
 
    CustomType(const Token& typename_token, const std::vector<TypeSignature>& generics);

    [[nodiscard]] bool is_generic() const override;

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::custom_type;
    };
    
    std::string type_name;
    std::vector<TypeSignature> type_parameters;
    std::string package_prefix;
};

struct InlineUnion : public TypeSignatureBody {

    InlineUnion(const DebugInformationsAwareEntity& typename_token, const std::vector<TypeSignature>& alternatives);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::inline_union;
    };

    [[nodiscard]] bool is_generic() const override;
    std::vector<TypeSignature> alternatives;
};

struct TemplateType : public TypeSignatureBody {

    TemplateType(const Token& typename_token); 

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::template_type;
    };

    [[nodiscard]] bool is_generic() const override;

    std::string type_name;
};

struct PrimitiveType : public TypeSignatureBody {
 
    PrimitiveType(const Token& typename_token);
    
    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::primitive_type;
    };

    [[nodiscard]] bool is_generic() const override;

    std::string type_name;
};

struct PointerType : public TypeSignatureBody {
    
    PointerType(const Token& pointer_symbol_token, const TypeSignature& pointed);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::pointer_type;
    };

    [[nodiscard]] bool is_generic() const override;

    TypeSignature pointed_type;
};

struct ArrayType : public TypeSignatureBody {
    
    ArrayType(const Token& array_open_square_bracket_token, int length, const TypeSignature& stored);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::array_type;
    };

    [[nodiscard]] bool is_generic() const override;

    int array_length;
    TypeSignature stored_type;
};

struct SliceType : public TypeSignatureBody {
    
    SliceType(const Token& slice_symbol_token, const TypeSignature& stored);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const {
        return TypeSignatureBody::Kind::slice_type;
    };

    [[nodiscard]] bool is_generic() const override;   
    
    TypeSignature stored_type;
};