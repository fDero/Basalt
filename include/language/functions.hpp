//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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
		const std::string& package_prefix,
	    const std::vector<Expression>& args, 
	    const std::vector<TypeSignature>& instantiated_generics
	);

	FunctionCall(
	    const Token& function_call_token, 
	    const std::vector<Expression>& args, 
	    const std::vector<TypeSignature>& instantiated_generics
	);

	[[nodiscard]] ExpressionBody::Kind expression_kind() const override {
		return ExpressionBody::Kind::function_call;
	}

	[[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::function_call;
	}

	std::string package_prefix;
	std::string function_name;
	std::vector<TypeSignature> instantiated_generics;
	std::vector<Expression> arguments;

	const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const {
		return (const ExpressionBody&) *this;		
	};
};
                  