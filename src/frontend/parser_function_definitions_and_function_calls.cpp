//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "syntax/keywords.hpp"

FunctionDefinition Parser::parse_function_definition() {
    assert_token_matches(source_tokens, iterator++, func_keyword);
    ensure_there_are_still_tokens(source_tokens, iterator);
    FunctionDefinition function(*iterator);
    std::advance(iterator, 1);
    function.template_generics_names = parse_template_generics();
    function.arguments = parse_function_def_arguments();
    function.return_type = parse_function_return_type();
    function.code = parse_function_def_body();
    return function;
}

FunctionDefinition Parser::parse_extern_function_declaration() {
    assert_token_matches(source_tokens, iterator++, extern_keyword);
    ensure_there_are_still_tokens(source_tokens, iterator);
    FunctionDefinition function(*iterator);
    std::advance(iterator, 1);
    function.arguments = parse_function_def_arguments();
    function.return_type = parse_function_return_type();
    ensure_token_matches(source_tokens, iterator++, "=");
    Expression underlying_function_name = parse_string_literal();
    function.extern_implementation = 
        underlying_function_name.get<StringLiteral>().value;
    ensure_token_matches(source_tokens, iterator++, ";");
    return function;
}

FunctionCall Parser::parse_function_call() {
    std::string package_prefix = parse_package_prefix();
    assert_token_is_text(source_tokens, iterator);
    assert_identifier_is_properly_formatted(iterator);
    const Token& function_name_token = *( iterator++ );
    std::vector<TypeSignature> concrete_generics = parse_concrete_generics();
    assert_token_matches(source_tokens, iterator++, "(");
    std::vector<Expression> call_arguments = parse_function_call_arguments();
    return FunctionCall { function_name_token, package_prefix, call_arguments, concrete_generics };
}

std::vector<Expression> Parser::parse_function_call_arguments() {
    std::vector<Expression> call_arguments;
    while (iterator != source_tokens.end() && iterator->sourcetext != ")") {
        call_arguments.push_back(parse_expression());
        ensure_either_comma_or_closed_paren_for_function_args(source_tokens, iterator);
        std::advance(iterator, iterator->sourcetext == ",");
    }
    ensure_token_matches(source_tokens, iterator++, ")");
    return call_arguments;
}

std::vector<Statement> Parser::parse_function_def_body() {
    ensure_token_matches(source_tokens, iterator++, "{");
    std::vector<Statement> code;
    while (iterator->sourcetext != "}") {
        Statement statement = parse_statement();
        code.push_back(statement);
    }
    ensure_token_matches(source_tokens, iterator++, "}");
    return code;
}

std::optional<TypeSignature> Parser::parse_function_return_type() {
    if (iterator->sourcetext != "->") {
        return std::nullopt;
    }
    assert_token_matches(source_tokens, iterator++, "->");
    return parse_typesignature(); 
}

std::vector<FunctionDefinition::Argument> Parser::parse_function_def_arguments() {
    ensure_token_matches(source_tokens, iterator, "(");
    std::vector<FunctionDefinition::Argument> arguments;
    if (std::next(iterator) != source_tokens.end() && std::next(iterator)->sourcetext == ")") {   
        std::advance(iterator, 1);
    }
    while (iterator->sourcetext == "," || iterator->sourcetext == "(") {
        std::advance(iterator, 1);
        std::string argument_name = parse_identifier().get<Identifier>().name;
        ensure_token_matches(source_tokens, iterator++, ":");
        TypeSignature argument_type = parse_typesignature();
        arguments.push_back(FunctionDefinition::Argument { argument_name, argument_type });
        ensure_there_are_still_tokens(source_tokens, iterator);
    }
    ensure_token_matches(source_tokens, iterator++, ")");
    return arguments;
}