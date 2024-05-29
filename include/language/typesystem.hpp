
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/syntax.hpp"
#include "misc/aliases.hpp"


struct TypeSignatureBody : public DebugInformationsAwareEntity {

    TypeSignatureBody(const Token& token) 
        : DebugInformationsAwareEntity(token) {}

    [[nodiscard]] virtual bool is_generic(const TemplateGenerics& generic_names) const = 0;

    virtual void instantiate_generics(const GenericSubstitutionRuleSet&) = 0;
    virtual ~TypeSignatureBody() = default;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] bool is_generic(const TemplateGenerics& generic_names) const;

    void instantiate_generics(const GenericSubstitutionRuleSet& rules);
};

struct CustomType : public TypeSignatureBody {
 
    CustomType(const Token& typename_token, const ConcreteGenerics& generics);

    [[nodiscard]] bool is_generic(const TemplateGenerics& generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    std::string type_name;
    ConcreteGenerics instantiation_generics;
    std::string package_prefix;
};

struct TemplateType : public TypeSignatureBody {
 
    std::string type_name;

    TemplateType(const Token& typename_token);

    [[nodiscard]] bool is_generic(const TemplateGenerics& generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;
};

struct PrimitiveType : public TypeSignatureBody {
 
    std::string type_name;

    PrimitiveType(const Token& typename_token);

    [[nodiscard]] bool is_generic(const TemplateGenerics& generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;
};

struct PointerType : public TypeSignatureBody {
    PointerType(const Token& pointer_symbol_token, const TypeSignature& pointed);

    [[nodiscard]] bool is_generic(const TemplateGenerics& template_generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    TypeSignature pointed_type;
};

struct ArrayType : public TypeSignatureBody {
    ArrayType(const Token& array_open_square_bracket_token, int length, const TypeSignature& stored);

    [[nodiscard]] bool is_generic(const TemplateGenerics& generics_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    int array_length;
    TypeSignature stored_type;
};

struct SliceType : public TypeSignatureBody {
    SliceType(const Token& slice_symbol_token, const TypeSignature& stored);

    [[nodiscard]] bool is_generic(const TemplateGenerics& template_generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;
    
    TypeSignature stored_type;
};