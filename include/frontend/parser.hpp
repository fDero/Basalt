//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <stack>
#include <regex>

#include "frontend/tokenizer.hpp"
#include "frontend/file_representation.hpp"
#include "errors/parsing_errors.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "language/expressions.hpp"

class Parser {

    public:
        Parser(const TokenizedFile& tokens);
        [[nodiscard]] FileRepresentation parse_everything();

        [[nodiscard]] Expression parse_array_literal();
        [[nodiscard]] Expression parse_expression();
        [[nodiscard]] Expression parse_terminal_expression();
        [[nodiscard]] Expression parse_expression_wrapped_in_parenthesis();
        [[nodiscard]] Expression parse_expression_wrapped_in_square_brackets();
        [[nodiscard]] Expression parse_prefix_operator();
        [[nodiscard]] Expression parse_string_literal();
        [[nodiscard]] Expression parse_integer_literal();
        [[nodiscard]] Expression parse_floating_literal();
        [[nodiscard]] Expression parse_boolean_literal();
        [[nodiscard]] Expression parse_character_literal();
        [[nodiscard]] Expression parse_identifier();

        [[nodiscard]] Statement parse_statement();
        [[nodiscard]] Statement parse_variable_definition();
        [[nodiscard]] Statement parse_constant_definition();
        [[nodiscard]] Statement parse_if_statement();
        [[nodiscard]] Statement parse_while_loop();
        [[nodiscard]] Statement parse_until_loop();
        [[nodiscard]] Statement parse_return_keyword();
        [[nodiscard]] Statement parse_break_keyword();
        [[nodiscard]] Statement parse_continue_keyword();
        [[nodiscard]] Statement parse_non_keyword_initialized_statements();

        [[nodiscard]] TypeSignature parse_typesignature();
        [[nodiscard]] TypeSignature parse_simple_typesignature();
        [[nodiscard]] TypeSignature parse_inline_union(const Token& first_type_token, const TypeSignature& first_type);
        [[nodiscard]] TypeSignature parse_pointer_type();
        [[nodiscard]] TypeSignature parse_array_type();
        [[nodiscard]] TypeSignature parse_slice_type();
        [[nodiscard]] TypeSignature parse_custom_type();
        [[nodiscard]] TypeSignature parse_primitive_type();
        [[nodiscard]] TypeSignature parse_template_type();
      
        [[nodiscard]] std::vector<std::string> parse_template_generics();
        [[nodiscard]] std::vector<TypeSignature> parse_concrete_generics();
        
        [[nodiscard]] FunctionDefinition parse_function_definition();
        [[nodiscard]] FunctionCall parse_function_call();
        [[nodiscard]] std::vector<Expression> parse_function_call_arguments();

        [[nodiscard]] UnionDefinition parse_union_definition();
        [[nodiscard]] StructDefinition parse_struct_definition();
        [[nodiscard]] std::vector<FunctionDefinition::Argument> parse_function_def_arguments();
        [[nodiscard]] std::optional<TypeSignature> parse_function_return_type();
        [[nodiscard]] std::vector<Statement> parse_function_def_body();
        [[nodiscard]] StructDefinition::Field parse_struct_field();

        [[nodiscard]] std::string parse_package_prefix();
        [[nodiscard]] std::string parse_package_name();
        [[nodiscard]] std::string parse_package_import();
        [[nodiscard]] TypeAlias parse_type_alias();
        void parse_source_code(FileRepresentation& output);
        void parse_alias_section(FileRepresentation& output);
        void parse_import_section(FileRepresentation& output);

    private:
        std::vector<std::string> template_generics_encountered_so_far;
        std::vector<Token> source_tokens;
        std::vector<Token>::iterator iterator;
        std::string filename;

        [[nodiscard]] std::vector<Statement> parse_code_block();
        [[nodiscard]] std::vector<Statement> parse_multiline_code_block();
        [[nodiscard]] std::optional<Expression> parse_value_after_object_declaration();
        [[nodiscard]] TypeSignature parse_type_after_object_declaration();

        [[nodiscard]] bool is_binary_operator();
        [[nodiscard]] bool is_square_bracket();
        [[nodiscard]] bool is_type_operator();
        [[nodiscard]] bool is_dot_member_access();
        [[nodiscard]] bool expression_ended();

        [[nodiscard]] Expression compose_dot_member_access(const Expression&);
        [[nodiscard]] Expression compose_binary_operator(const Expression&);
        [[nodiscard]] Expression compose_square_bracket_access(const Expression&);
        [[nodiscard]] Expression compose_type_operator(const Expression&);
        
        [[nodiscard]] Expression rotate_binary_operator_to_match_operators_priority(const BinaryOperator&);
        [[nodiscard]] Expression rotate_binary_operator_on_binary_operator_if_needed(const BinaryOperator&, const BinaryOperator&);
        [[nodiscard]] Expression rotate_binary_operator_on_unary_operator_if_needed(const BinaryOperator&, const UnaryOperator&);
        [[nodiscard]] Expression rotate_to_match_type_operator_priority(const TypeOperator&);
        [[nodiscard]] Expression rotate_to_match_dot_member_access_priority(const DotMemberAccess&);
        [[nodiscard]] Expression rotate_to_match_square_brackets_access_priority(const SquareBracketsAccess&);

        [[nodiscard]] bool is_template_type(const std::string& type_name);
};