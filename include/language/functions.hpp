
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
	    const std::vector<TypeSignature>& instantiated_generics
	);

	std::string function_name;
	std::vector<TypeSignature> instantiated_generics;
	std::vector<Expression> arguments;
};

struct PrecompiledFunctionCall {

	PrecompiledFunctionCall(
	    const FunctionCall& original_function_call, 
	    const std::vector<TypeSignature>& arguments_types,
		const std::string& filename
	);

	std::string filename;
	std::string package_prefix;
	const FunctionCall& original_function_call;	
	std::vector<TypeSignature> arguments_types;
};