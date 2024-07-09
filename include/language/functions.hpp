
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

	const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const {
		return (const ExpressionBody&) *this;		
	};
};
