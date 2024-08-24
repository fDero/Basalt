
#pragma once
#include "language/definitions.hpp"

struct UnionDefinitionFactory {

    inline static const std::vector<std::string> no_generics = {};
    inline static const std::vector<StructDefinition::Field> no_fields = {}; 

    static UnionDefinition make_union_definition(
        const std::string& union_name, 
        const std::string& filename,
        const std::vector<std::string> generics, 
        const std::vector<TypeSignature>& types
    ) {
        UnionDefinition new_union_definition {
            Token {
                union_name, filename, 1, 2, 5,
                Token::Type::union_keyword
            },
        };
        new_union_definition.types = types;
        new_union_definition.template_generics_names = generics;
        return new_union_definition;
    }
};