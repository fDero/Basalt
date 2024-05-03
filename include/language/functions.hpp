
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
	    const std::string& name, 
	    const std::vector<Expression>& args, 
	    const std::vector<TypeSignature>& instaciated_generics
	) 
		: function_name(name)
		, instaciated_generics(instaciated_generics)
		, arguments(args) {}
};