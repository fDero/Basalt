/**
 * @file typesignatures.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the different kinds of typesignatures in the language.
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "frontend/syntax.hpp"
#include "misc/forward_declarations.hpp"

/**
 * @brief   Real base class for all the typesignatures in the language.
 * 
 * @details Base class for all the typesignatures in the language. Used togheter
 *          with the TypeSignature class to create a value-semantics wrapper around
 *          the different kinds of typesignatures in a polymorphic way.
 * 
 * @note    This struct is not meant to be used directly, instead you should use the TypeSignature class,
 *          for wich this class is just the internal implementation.
 * 
 * @note    Since this struct extends DebugInformationsAwareEntity, all the typesignatures in the language
 *          are aware of their position in the source code file.
 *          
 * @see     TypeSignature
 * @see     Polymorph
 */
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

/**
 * @brief   Value-semantics polymorphism class to represent all the typesignatures in the language.
 * 
 * @details the TypeSignature class is used togheter with the TypeSignatureBody class to 
 *          create a value-semantics wrapper around the different kinds of typesignatures in a polymorphic way.
 * 
 * @see     TypeSignatureBody
 * @see     Polymorph
 */
struct TypeSignature : public Polymorph<TypeSignatureBody> {

    using Polymorph<TypeSignatureBody>::is;
    using Polymorph<TypeSignatureBody>::get;
    using Polymorph<TypeSignatureBody>::Polymorph;

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const ;
    [[nodiscard]] bool is_generic() const;
};

/**
 * @brief    Used to represent an user-defined-type in the source-code.
 * 
 * @details The CustomType struct is used to represent an user-defined-type in the source-code,
 *          like a struct, union, or type alias. It contains the name of the type, and the type parameters.
 * 
 * @note    Depending on the parsing context, a non-pointer, non-slice, non-array, non-primitive, non-inline type
 *          can be either a CustomType, or a TemplateType. This is done to allow the type checker to distinguish
 *          between a generic type parameter, and a concrete type way faster than if it had to check if the type
 *          is a CustomType, and then check if it's a generic type parameter. This is possible because the
 *          parser is aware of the context in which the type is being parsed, and can choose the right type
 *          to represent the type signature.
 * 
 * @see     StructDefinition
 * @see     UnionDefinition
 * @see     TypeAlias
 */
struct CustomType : public TypeSignatureBody {

    virtual ~CustomType() = default;

    CustomType(const Token& typename_token, const std::vector<TypeSignature>& generics);

    [[nodiscard]] bool is_generic() const override;
    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    
    std::string type_name;
    std::vector<TypeSignature> type_parameters;
    std::string package_prefix;
};

/**
 * @brief   Used to represent an inline-union in the source-code.
 * 
 * @details The InlineUnion struct is used to represent an inline-union in the source-code,
 *          it contains just the type-alternatives of the union.
 * 
 * @note    An inline-union can not be generic, since it's not a type definition, but just a type signature.
 * 
 * @note    The whole idea of an inline-union is to allow type-inference to infer the type of two function argument
 *          that are supposed to be the same, but are different types. This is done by creating an inline-union
 *          of the two types, and then using the inline-union as the type of the two arguments.
 * 
 * @see     AssignmentTypeChecker
 * @see     UnionDefinition
 */
struct InlineUnion : public TypeSignatureBody {

    virtual ~InlineUnion() = default;

    InlineUnion(const DebugInformationsAwareEntity& typename_token, const std::vector<TypeSignature>& alternatives);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    std::vector<TypeSignature> alternatives;
};

/**
 * @brief   Used to represent a template type in the source-code.
 * 
 * @details The TemplateType struct is used to represent a template type in the source-code,
 *          it contains the name of the type, and it's supposed to be used as a generic type parameter.
 *          
 * @note    A TemplateType is always generic, since it's supposed to be used as a generic type parameter.
 *          
 * @note    Depending on the parsing context, a non-pointer, non-slice, non-array, non-primitive, non-inline type
 *          can be either a CustomType, or a TemplateType. This is done to allow the type checker to distinguish
 *          between a generic type parameter, and a concrete type way faster than if it had to check if the type
 *          is a CustomType, and then check if it's a generic type parameter. This is possible because the
 *          parser is aware of the context in which the type is being parsed, and can choose the right type
 *          to represent the type signature.
 * 
 * @see     GenericSubstitutionRule
 * @see     GenericSubstitutionRule::Set
 * @see     GenericSubstitutionRule::Set::Ref
 * @see     GenericsInstantiationEngine
 */
struct TemplateType : public TypeSignatureBody {

    virtual ~TemplateType() = default;

    TemplateType(const Token& typename_token); 

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    std::string type_name;
};

/**
 * @brief   Used to represent a primitive type in the source-code.
 * 
 * @details The PrimitiveType struct is used to represent a primitive type in the source-code,
 *          like Int, Float, Bool etc. It contains just the name of the type.
 */
struct PrimitiveType : public TypeSignatureBody {
 
    virtual ~PrimitiveType() = default;

    PrimitiveType(const Token& typename_token);
    PrimitiveType(const std::string& type_name, const DebugInformationsAwareEntity& debug_info);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    std::string type_name;
};

/**
 * @brief   Used to represent a pointer type in the source-code.
 * 
 * @details The PointerType struct is used to represent a pointer type in the source-code,
 *          it contains the type of the pointed value.
 */
struct PointerType : public TypeSignatureBody {
    
    PointerType(const DebugInformationsAwareEntity& pointer_symbol_token, const TypeSignature& pointed);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    TypeSignature pointed_type;
};

/**
 * @brief   Used to represent an array type in the source-code.
 * 
 * @details The ArrayType struct is used to represent an array type in the source-code,
 *          it contains the length of the array, and the type of the stored value.
 */
struct ArrayType : public TypeSignatureBody {
    
    ArrayType(const DebugInformationsAwareEntity& array_open_square_bracket_token, int length, const TypeSignature& stored);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;

    int array_length;
    TypeSignature stored_type;
};

/**
 * @brief   Used to represent a slice type in the source-code.
 * 
 * @details The SliceType struct is used to represent a slice type in the source-code,
 *          it contains jsut the type of the stored value.
 */
struct SliceType : public TypeSignatureBody {
    
    SliceType(const Token& slice_symbol_token, const TypeSignature& stored);

    [[nodiscard]] virtual TypeSignatureBody::Kind typesiganture_kind() const;
    [[nodiscard]] bool is_generic() const override;   
    
    TypeSignature stored_type;
};