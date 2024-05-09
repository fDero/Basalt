
#pragma once
#include "language/statements.hpp"
#include "language/expressions.hpp"

struct FunctionCall 
	: public ExpressionBody 
	, public StatementBody
{
	virtual ~FunctionCall() = default;
	FunctionCall(
	    const Token& function_call_token, 
	    const std::vector<Expression>& args, 
	    const std::vector<TypeSignature>& instaciated_generics
	);

	std::string function_name;
	ConcreteGenerics instaciated_generics;
	std::vector<Expression> arguments;
};