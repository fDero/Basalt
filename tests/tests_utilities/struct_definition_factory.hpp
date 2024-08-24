
#pragma once
#include "language/definitions.hpp"

struct StructDefinitionFactory {

    inline static const std::vector<std::string> no_generics = {};
    inline static const std::vector<StructDefinition::Field> no_fields = {}; 

    static StructDefinition make_struct_definition(
        const std::string& struct_name,
        const std::string& filename, 
        const std::vector<std::string> generics, 
        const std::vector<StructDefinition::Field>& fields
    ) {
        StructDefinition new_struct_definition {
            Token {
                struct_name, filename, 1, 2, 5,
                Token::Type::text
            },
        };
        new_struct_definition.fields = fields;
        new_struct_definition.template_generics_names = generics;
        return new_struct_definition;
    }
};