
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/syntax.hpp"
#include "misc/forward_declarations.hpp"

struct TypeSignatureBody : public DebugInformationsAwareEntity {

    TypeSignatureBody(const DebugInformationsAwareEntity& token)
        : DebugInformationsAwareEntity(token) {}

    [[nodiscard]] virtual bool is_generic() const = 0;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] bool is_generic() const;
};

struct CustomType : public TypeSignatureBody {
 
    CustomType(const Token& typename_token, const std::vector<TypeSignature>& generics);

    [[nodiscard]] bool is_generic() const override;
    
    std::string type_name;
    std::vector<TypeSignature> type_parameters;
    std::string package_prefix;
};

struct InlineUnion : public TypeSignatureBody {

    InlineUnion(const DebugInformationsAwareEntity& typename_token, const std::vector<TypeSignature>& alternatives);

    [[nodiscard]] bool is_generic() const override;
    std::vector<TypeSignature> alternatives;
};

struct TemplateType : public TypeSignatureBody {
 
    std::string type_name;

    TemplateType(const Token& typename_token);
    [[nodiscard]] bool is_generic() const override;
};

struct PrimitiveType : public TypeSignatureBody {
 
    std::string type_name;

    PrimitiveType(const Token& typename_token);
    [[nodiscard]] bool is_generic() const override;
};

struct PointerType : public TypeSignatureBody {
    PointerType(const Token& pointer_symbol_token, const TypeSignature& pointed);

    [[nodiscard]] bool is_generic() const override;
    TypeSignature pointed_type;
};

struct ArrayType : public TypeSignatureBody {
    ArrayType(const Token& array_open_square_bracket_token, int length, const TypeSignature& stored);

    [[nodiscard]] bool is_generic() const override;

    int array_length;
    TypeSignature stored_type;
};

struct SliceType : public TypeSignatureBody {
    SliceType(const Token& slice_symbol_token, const TypeSignature& stored);

    [[nodiscard]] bool is_generic() const override;   
    TypeSignature stored_type;
};