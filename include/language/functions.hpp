
#pragma once
#include "language/statements.hpp"
#include "language/expressions.hpp"

struct FunctionCall 
	: public ExpressionBody 
	, public StatementBody
{
	std::string function_name;
	std::vector<TypeSignature> instaciated_generics;
	std::vector<Expression> arguments;
	FunctionCall(
	    const Token& function_call_token, 
	    const std::vector<Expression>& args, 
	    const std::vector<TypeSignature>& instaciated_generics
	) 
		: ExpressionBody(function_call_token)
		, function_name(function_call_token.sourcetext)
		, instaciated_generics(instaciated_generics)
		, arguments(args) {}
};