
#pragma once
#include "toolchain/tokenizer.hpp"
#include "language/syntax.hpp"
#include "language/typesystem.hpp"
#include "language/definitions.hpp"
#include "toolchain/representation.hpp"
#include <vector>
#include "errors/error-types.hpp"
#include <string>
#include <map>
#include <unordered_map>

[[noreturn]] void assert_unreachable();

void assert_integer_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_floating_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_character_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_boolean_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_string_literal_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_identifier_is_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_type_is_properly_formatted(const std::vector<Token>::iterator& iterator);

void assert_token_is_prefix_operator(const std::vector<Token>::iterator& iterator);

void assert_token_is_binary_operator(const std::vector<Token>::iterator& iterator);

void assert_token_is_type_operator(const std::vector<Token>::iterator& iterator);

void assert_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator, 
    const std::string& text
);

void assert_token_is_string_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_boolean_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_character_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_floating_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_integer_literal(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_token_is_text(
    const std::vector<Token>& source_tokens, 
    const std::vector<Token>::iterator& iterator
);

void assert_tokens_not_ended(
    const std::vector<Token>::iterator& iterator, 
    const std::vector<Token>& tokens
);

template<typename T>
inline void assert_typesignature_is(const TypeSignature& typesignature) {
    #ifdef DEBUG_BUILD
    if (!typesignature.is<T>()) {
        std::string T_type_converted_to_string = typeid(T).name();
        throw InternalError {
            "somehow a typesignature was expected to be a " + 
            T_type_converted_to_string + " instead it wasn't"    
        };
    }
    #endif
}

void assert_vectors_have_same_size_hence_they_can_be_zipped(
    const std::vector<std::string>& template_generics,
    const std::vector<TypeSignature>& type_parameters
);

void assert_instantiation_struct_is_compatible_with_template_struct(
    const CustomType& concrete_type,
    const StructDefinition& template_struct
);

void assert_instantiation_union_is_compatible_with_template_union(
    const CustomType& concrete_type,
    const UnionDefinition& template_union
);

void assert_instantiation_union_is_compatible_with_template_alias(
    const CustomType& concrete_type,
    const TypeAlias& template_alias
);

void assert_packagename_is_found(
    const std::unordered_map<std::string, std::string>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::string>::const_iterator& end
);

void assert_files_vector_is_found(
    const std::unordered_map<std::string, std::vector<FileRepresentation>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<FileRepresentation>>::const_iterator& end
);

void assert_imports_vector_is_found(
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<std::string>>::const_iterator& end
);

void assert_overload_set_exists(
    const std::unordered_map<std::string, std::vector<FunctionDefinition::Ref>>::const_iterator& search_outcome,
    const std::unordered_map<std::string, std::vector<FunctionDefinition::Ref>>::const_iterator& end
);