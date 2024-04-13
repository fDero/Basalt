
#pragma once
#include "misc/polymorph.hpp"
#include "language/syntax.hpp"
#include <string>
#include <vector>

struct TypeSignatureBody {

    [[nodiscard]] virtual bool is_primitive_type() const = 0;

    [[nodiscard]] virtual bool is_generic(
        const std::vector<std::string>& template_generic_names) const = 0;

    [[nodiscard]] virtual std::string to_string() const = 0;

    [[nodiscard]] virtual std::string function_retrieval_match_string(
        const std::vector<std::string>& template_generics_names) const = 0;

    [[nodiscard]] virtual std::string struct_retrieval_match_string() const = 0;

    virtual ~TypeSignatureBody() = default;
};

struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] std::string to_string() const { 
        return ptr->to_string(); 
    }

    [[nodiscard]] std::string function_retrieval_match_string
        (const std::vector<std::string>& template_generics_names) const { 
            return ptr->function_retrieval_match_string(template_generics_names); 
    }

    [[nodiscard]] std::string struct_retrieval_match_string() 
        const { return ptr->struct_retrieval_match_string(); }

    bool is_primitive_type() const { return ptr->is_primitive_type(); }

    bool is_generic(const std::vector<std::string>& template_generic_names) 
        const { return ptr->is_generic(template_generic_names); }
};

struct BaseType : public TypeSignatureBody {
    std::string type_name;
    std::vector<TypeSignature> instanciated_generics;
    BaseType(const std::string& name, const std::vector<TypeSignature>& generics)
        : type_name(name), instanciated_generics(generics) {}

    std::string to_string() const override;
    
    std::string function_retrieval_match_string(
        const std::vector<std::string>& template_generics_names) const override;
    
    std::string struct_retrieval_match_string() const override;

    bool is_primitive_type() const override;

    bool is_generic(const std::vector<std::string>& template_generic_names) const override;
};

struct PointerType : public TypeSignatureBody {
    TypeSignature pointed_type;
    PointerType(const TypeSignature& pointed)
        : pointed_type(pointed) {}

    std::string to_string() const override;

    std::string function_retrieval_match_string(
        const std::vector<std::string>& template_generics_names) const override;

    std::string struct_retrieval_match_string() const override;

    bool is_primitive_type() const override;

    bool is_generic(const std::vector<std::string>& template_generic_names) const override;
};

struct ArrayType : public TypeSignatureBody {
    int array_length;
    TypeSignature stored_type;
    ArrayType(int length, const TypeSignature& stored)
        : stored_type(stored), array_length(length) {}

    std::string to_string() const override;

    std::string function_retrieval_match_string(
        const std::vector<std::string>& template_generics_names) const override;

    std::string struct_retrieval_match_string() const override;

    bool is_primitive_type() const override;

    bool is_generic(const std::vector<std::string>& template_generic_names) const override;
};

struct SliceType : public TypeSignatureBody {
    TypeSignature stored_type;
    SliceType(const TypeSignature& stored)
        : stored_type(stored) {}

    std::string to_string() const override;

    std::string function_retrieval_match_string(
        const std::vector<std::string>& template_generics_names) const override;

    std::string struct_retrieval_match_string() const override;

    bool is_primitive_type() const override;

    bool is_generic(const std::vector<std::string>& template_generic_names) const override;
};