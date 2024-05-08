
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/syntax.hpp"
#include <string>
#include <vector>

struct GenericSubstitutionRuleSet;

struct TypeSignatureBody : public DebugInformationsAwareEntity {

    TypeSignatureBody(const Token& token) 
        : DebugInformationsAwareEntity(token) {}

    [[nodiscard]] virtual bool is_generic(const std::vector<std::string>& generic_names) const = 0;
    [[nodiscard]] virtual std::string to_string() const = 0;
    [[nodiscard]] virtual std::string to_match_string() const = 0;

    virtual void instantiate_generics(const GenericSubstitutionRuleSet&) = 0;
    virtual ~TypeSignatureBody() = default;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] std::string to_match_string() const;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generic_names) const;
    [[nodiscard]] bool is_primitive_type() const;

    void instantiate_generics(const GenericSubstitutionRuleSet& rules);
};

struct BaseType : public TypeSignatureBody {
 
    BaseType(const std::string& package_prefix, const Token& typename_token, const std::vector<TypeSignature>& generics);

    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    std::string type_name;
    std::vector<TypeSignature> instantiationd_generics;
    std::string package_prefix;
};

struct PointerType : public TypeSignatureBody {
    PointerType(const Token& pointer_symbol_token, const TypeSignature& pointed);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& template_generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    TypeSignature pointed_type;
};

struct ArrayType : public TypeSignatureBody {
    ArrayType(const Token& array_open_square_bracket_token, int length, const TypeSignature& stored);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generics_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    int array_length;
    TypeSignature stored_type;
};

struct SliceType : public TypeSignatureBody {
    SliceType(const Token& slice_symbol_token, const TypeSignature& stored);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& template_generic_names) const override;
    void instantiate_generics(const GenericSubstitutionRuleSet&) override;

    TypeSignature stored_type;
};