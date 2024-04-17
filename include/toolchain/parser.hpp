
#pragma once
#include "toolchain/tokenizer.hpp"
#include "toolchain/rappresentation.hpp"
#include "errors/parsing_errors.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "language/expressions.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <stack>
#include <regex>

class Parser {

    public:
        Parser(const std::vector<Token>& tokens);
        [[nodiscard]] FileRappresentation parse_everything();

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
        [[nodiscard]] TypeSignature parse_pointer_type();
        [[nodiscard]] TypeSignature parse_array_type();
        [[nodiscard]] TypeSignature parse_slice_type();
        [[nodiscard]] TypeSignature parse_base_type();
      
        [[nodiscard]] std::vector<std::string> parse_template_generics();
        [[nodiscard]] std::vector<TypeSignature> parse_concrete_generics();
        
        [[nodiscard]] FunctionDefinition parse_function_definition();
        [[nodiscard]] FunctionCall parse_function_call();
        [[nodiscard]] std::vector<Expression> parse_function_call_arguments();
       
        [[nodiscard]] StructDefinition parse_struct_definition();
        [[nodiscard]] std::vector<FunctionDefinition::Argument> parse_function_def_arguments();
        [[nodiscard]] std::optional<TypeSignature> parse_function_return_type();
        [[nodiscard]] std::vector<Statement> parse_function_def_body();
        [[nodiscard]] StructDefinition::Field parse_struct_field();

    private:
        std::vector<Token> source_tokens;
        std::vector<Token>::iterator iterator;

        [[nodiscard]] std::vector<Statement> parse_code_block();
        [[nodiscard]] std::vector<Statement> parse_multiline_code_block();
        [[nodiscard]] std::optional<Expression> parse_value_after_object_declaration();
        [[nodiscard]] std::optional<TypeSignature> parse_type_after_object_declaration();

        [[nodiscard]] bool is_binary_operator();
        [[nodiscard]] bool is_square_bracket();
        [[nodiscard]] bool expression_ended();

        [[nodiscard]] TypeSignature compose_nested_type(const TypeSignature&);
        [[nodiscard]] Expression compose_binary_operator(const Expression&);
        [[nodiscard]] Expression compose_square_bracket_access(const Expression&);
        
        [[nodiscard]] Expression rotate_binary_operator_to_match_operators_priority(const BinaryOperator&);
        [[nodiscard]] Expression rotate_binary_operator_on_binary_operator_if_needed(const BinaryOperator&, const BinaryOperator&);
        [[nodiscard]] Expression rotate_binary_operator_on_unary_operator_if_needed(const BinaryOperator&, const UnaryOperator&);
};