
#pragma once
#include "language/definitions.hpp"

struct TypeAliasFactory {

    inline static const std::vector<std::string> no_generics = {};

    static TypeAlias make_type_alias(
        const std::string& alias_name, 
        const std::string& filename,
        const std::vector<std::string> generics, 
        const TypeSignature& aliased_type
    ) {
        TypeAlias new_type_alias {
            Token {
                alias_name, filename, 1, 2, 5,
                Token::Type::struct_keyword
            },
            generics,
            aliased_type
        };
        return new_type_alias;
    }
};