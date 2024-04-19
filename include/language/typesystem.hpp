
#pragma once
#include "misc/polymorph.hpp"
#include "language/syntax.hpp"
#include <string>
#include <vector>

struct GenericSubstitutionRuleSet;

struct TypeSignatureBody {

    [[nodiscard]] virtual bool is_core_language_type() const = 0;
    [[nodiscard]] virtual bool is_generic(const std::vector<std::string>& generic_names) const = 0;
    [[nodiscard]] virtual std::string to_string() const = 0;
    [[nodiscard]] virtual std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const = 0;
    [[nodiscard]] virtual std::string to_match_string() const = 0;

    virtual void instanciate_generics(const GenericSubstitutionRuleSet&) = 0;
    virtual ~TypeSignatureBody() = default;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const;
    [[nodiscard]] std::string to_match_string() const;
    [[nodiscard]] bool is_core_language_type() const;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generic_names) const;

    void instanciate_generics(const GenericSubstitutionRuleSet& rules);
};

struct PrimitiveType : public TypeSignatureBody {
    PrimitiveType(const std::string& name);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_core_language_type() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generic_names) const override;
    void instanciate_generics(const GenericSubstitutionRuleSet&) override;

    std::string type_name;
};

struct CustomType : public TypeSignatureBody {
    CustomType(const std::string& name, const std::vector<TypeSignature>& generics);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_core_language_type() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generic_names) const override;
    void instanciate_generics(const GenericSubstitutionRuleSet&) override;

    std::string type_name;
    std::vector<TypeSignature> instanciated_generics;
};

struct PointerType : public TypeSignatureBody {
    PointerType(const TypeSignature& pointed);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_core_language_type() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& template_generic_names) const override;
    void instanciate_generics(const GenericSubstitutionRuleSet&) override;

    TypeSignature pointed_type;
};

struct ArrayType : public TypeSignatureBody {
    ArrayType(int length, const TypeSignature& stored);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_core_language_type() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& generics_names) const override;
    void instanciate_generics(const GenericSubstitutionRuleSet&) override;

    int array_length;
    TypeSignature stored_type;
};

struct SliceType : public TypeSignatureBody {
    SliceType(const TypeSignature& stored);
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string function_retrieval_match_string(const std::vector<std::string>& generics_names) const override;
    [[nodiscard]] std::string to_match_string() const override;
    [[nodiscard]] bool is_core_language_type() const override;
    [[nodiscard]] bool is_generic(const std::vector<std::string>& template_generic_names) const override;
    void instanciate_generics(const GenericSubstitutionRuleSet&) override;

    TypeSignature stored_type;
};