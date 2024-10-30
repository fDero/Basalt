//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "frontend/syntax.hpp"
#include "misc/forward_declarations.hpp"

struct TypeSignatureBody : public DebugInformationsAwareEntity {

    enum class Kind {
        custom_type,
        inline_union,
        template_type,
        primitive_type,
        pointer_type,
        array_type,
        slice_type
    };

    virtual ~TypeSignatureBody() = default;

    TypeSignatureBody(const DebugInformationsAwareEntity& token)
        : DebugInformationsAwareEntity(token) {}

    [[nodiscard]] virtual Kind typesiganture_kind() const = 0;
    [[nodiscard]] virtual bool is_generic() const = 0;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const ;
    [[nodiscard]] bool is_generic() const;
};

struct CustomType : public TypeSignatureBody {

    virtual ~CustomType() = default;

    CustomType(const Token& typename_token, const std::vector<TypeSignature>& generics);

    [[nodiscard]] bool is_generic() const override;
    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    
    std::string type_name;
    std::vector<TypeSignature> type_parameters;
    std::string package_prefix;
};

struct InlineUnion : public TypeSignatureBody {

    virtual ~InlineUnion() = default;

    InlineUnion(const DebugInformationsAwareEntity& typename_token, const std::vector<TypeSignature>& alternatives);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    std::vector<TypeSignature> alternatives;
};

struct TemplateType : public TypeSignatureBody {

    virtual ~TemplateType() = default;

    TemplateType(const Token& typename_token); 

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    std::string type_name;
};

struct PrimitiveType : public TypeSignatureBody {
 
    virtual ~PrimitiveType() = default;

    PrimitiveType(const Token& typename_token);
    PrimitiveType(const std::string& type_name, const DebugInformationsAwareEntity& debug_info);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    std::string type_name;
};

struct PointerType : public TypeSignatureBody {
    
    PointerType(const DebugInformationsAwareEntity& pointer_symbol_token, const TypeSignature& pointed);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    TypeSignature pointed_type;
};

struct ArrayType : public TypeSignatureBody {
    
    ArrayType(const DebugInformationsAwareEntity& array_open_square_bracket_token, size_t length, const TypeSignature& stored);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    size_t array_length;
    TypeSignature stored_type;
};

struct SliceType : public TypeSignatureBody {
    
    SliceType(const Token& slice_symbol_token, const TypeSignature& stored);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;   
    
    TypeSignature stored_type;
};