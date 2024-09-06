
#pragma once
#include "language/typesignatures.hpp"

struct TypeSignatureFactory {

    inline static const TypeSignature Int       = PrimitiveType{ Token { "Int",       "test.basalt", 1, 1, 3, Token::Type::type}   };
    inline static const TypeSignature Float     = PrimitiveType{ Token { "Float",     "test.basalt", 1, 1, 5, Token::Type::type}   };
    inline static const TypeSignature String    = PrimitiveType{ Token { "String",    "test.basalt", 1, 1, 6, Token::Type::type}   };
    inline static const TypeSignature Bool      = PrimitiveType{ Token { "Bool",      "test.basalt", 1, 1, 4, Token::Type::type}   };
    inline static const TypeSignature Char      = PrimitiveType{ Token { "Char",      "test.basalt", 1, 1, 4, Token::Type::type}   };
    inline static const TypeSignature RawString = PrimitiveType{ Token { "RawString", "test.basalt", 1, 1, 9, Token::Type::type}   };

    inline static const TypeSignature T = TemplateType{ Token { "T", "test.basalt", 1, 1, 1, Token::Type::type} };
    inline static const TypeSignature U = TemplateType{ Token { "U", "test.basalt", 1, 1, 1, Token::Type::type} };
    inline static const TypeSignature V = TemplateType{ Token { "V", "test.basalt", 1, 1, 1, Token::Type::type} };
    inline static const TypeSignature K = TemplateType{ Token { "K", "test.basalt", 1, 1, 1, Token::Type::type} };
    inline static const TypeSignature S = TemplateType{ Token { "S", "test.basalt", 1, 1, 1, Token::Type::type} };

    inline static const TypeSignature PtrToT = PointerType { Token { "#", "test.basalt", 1, 1, 1, Token::Type::symbol}, T };
    inline static const TypeSignature PtrToU = PointerType { Token { "#", "test.basalt", 1, 1, 1, Token::Type::symbol}, U };
    inline static const TypeSignature PtrToV = PointerType { Token { "#", "test.basalt", 1, 1, 1, Token::Type::symbol}, V };
    inline static const TypeSignature PtrToK = PointerType { Token { "#", "test.basalt", 1, 1, 1, Token::Type::symbol}, K };
    inline static const TypeSignature PtrToS = PointerType { Token { "#", "test.basalt", 1, 1, 1, Token::Type::symbol}, S };

    inline static const TypeSignature ListOfStrings       = CustomType{ Token { "List", "test.basalt", 1, 1, 4, Token::Type::type}, { String } };
    inline static const TypeSignature ListOfInts          = CustomType{ Token { "List", "test.basalt", 1, 1, 4, Token::Type::type}, { Int } };
    inline static const TypeSignature PairOfStringAndInt  = CustomType{ Token { "Pair", "test.basalt", 1, 1, 4, Token::Type::type}, { String, Int } };
    inline static const TypeSignature PairOfStringAndChar = CustomType{ Token { "Pair", "test.basalt", 1, 1, 4, Token::Type::type}, { String, Char } };

    inline static const TypeSignature PointerToInt = PointerType{ Token { "#", "test.basalt", 1, 1, 4, Token::Type::type}, Int };
    inline static const TypeSignature PointerToFloat = PointerType{ Token { "#", "test.basalt", 1, 1, 4, Token::Type::type}, Float };
    inline static const TypeSignature PointerToString = PointerType{ Token { "#", "test.basalt", 1, 1, 4, Token::Type::type}, String };
    inline static const TypeSignature PointerToBool = PointerType{ Token { "#", "test.basalt", 1, 1, 4, Token::Type::type}, Bool };
    inline static const TypeSignature PointerToChar = PointerType{ Token { "#", "test.basalt", 1, 1, 4, Token::Type::type}, Char };    

    inline static const TypeSignature ArrayOfInts = ArrayType{ Token { "[", "test.basalt", 1, 1, 4, Token::Type::type}, 10, Int };
    inline static const TypeSignature ArrayOfFloats = ArrayType{ Token { "[", "test.basalt", 1, 1, 4, Token::Type::type}, 10, Float };
    inline static const TypeSignature ArrayOfStrings = ArrayType{ Token { "[", "test.basalt", 1, 1, 4, Token::Type::type}, 10, String };
    inline static const TypeSignature ArrayOfBools = ArrayType{ Token { "[", "test.basalt", 1, 1, 4, Token::Type::type}, 10, Bool };
    inline static const TypeSignature ArrayOfChars = ArrayType{ Token { "[", "test.basalt", 1, 1, 4, Token::Type::type}, 10, Char };

    inline static const TypeSignature SliceOfInts = SliceType{ Token { "[]", "test.basalt", 1, 1, 4, Token::Type::type}, Int };
    inline static const TypeSignature SliceOfFloats = SliceType{ Token { "[]", "test.basalt", 1, 1, 4, Token::Type::type}, Float };
    inline static const TypeSignature SliceOfStrings = SliceType{ Token { "[]", "test.basalt", 1, 1, 4, Token::Type::type}, String };
    inline static const TypeSignature SliceOfBools = SliceType{ Token { "[]", "test.basalt", 1, 1, 4, Token::Type::type}, Bool };
    inline static const TypeSignature SliceOfChars = SliceType{ Token { "[]", "test.basalt", 1, 1, 4, Token::Type::type}, Char };

    inline static const TypeSignature IntOrFloat = InlineUnion {
        Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type },
        { TypeSignatureFactory::Int, TypeSignatureFactory::Float }
    };

    inline static const TypeSignature PointerToIntOrFloat = PointerType {
        Token { "#", "test.basalt", 1, 1, 1, Token::Type::type },
        IntOrFloat
    };

    inline static const TypeSignature SliceOfIntsOrFloats = SliceType {
        Token { "$", "test.basalt", 1, 1, 1, Token::Type::type },
        IntOrFloat
    };

    inline static const TypeSignature PrimitiveTypesUnion = InlineUnion {
        Token { "Int", "test.basalt", 1, 1, 1, Token::Type::type },
        { 
            TypeSignatureFactory::Int, 
            TypeSignatureFactory::Float, 
            TypeSignatureFactory::String,  
            TypeSignatureFactory::RawString,
            TypeSignatureFactory::Bool, 
            TypeSignatureFactory::Char
        }
    };

    inline static const TypeSignature TorU = InlineUnion {
        Token { "T", "test.basalt", 1, 1, 1, Token::Type::type },
        { TypeSignatureFactory::T, TypeSignatureFactory::U }
    };


    static TypeSignature make_custom_type(const std::string& type_name, const std::vector<TypeSignature>& generics) {
        return CustomType{ 
            Token { 
                type_name, "test.basalt", 1, 1, 
                static_cast<unsigned int>(type_name.size()), 
                Token::Type::type
            }, 
            generics 
        };
    }

    static TypeSignature make_ptr_type(const TypeSignature& pointed) {
        return PointerType { 
            Token { "#", "test.basalt", 1, 1, 1, Token::Type::type }, 
            pointed
        };
    }

    static TypeSignature make_slice_type(const TypeSignature& stored) {
        return SliceType { 
            Token { "$", "test.basalt", 1, 1, 1, Token::Type::type }, 
            stored
        };
    }

    static TypeSignature make_array_type(const TypeSignature& stored, size_t length) {
        return ArrayType { 
            Token { "[", "test.basalt", 1, 1, 1, Token::Type::type }, 
            length,
            stored
        };
    }
};