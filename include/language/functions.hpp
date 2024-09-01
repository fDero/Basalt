/**
 * @file functions.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the FunctionCall struct
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024 
 * 
 * @details The reason the content of this file has been extrapolated from expressions.hpp and statements.hpp is that
 *          the FunctionCall struct is a common entity between the two files, and it is used to represent a function call.
 *
 */

#pragma once

#include "language/statements.hpp"
#include "language/expressions.hpp"

/**
 * @brief   This struct represents a function call in the source-code.
 * 
 * @details The FunctionCall struct is used to represent a function call in the source-code,
 *          it contains the name of the function, the arguments of the function, and the instantiated generics.
 *          
 */
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

	[[nodiscard]] ExpressionBody::Kind expression_kind() const override {
		return ExpressionBody::Kind::function_call;
	}

	[[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::function_call;
	}

	std::string function_name;
	std::vector<TypeSignature> instantiated_generics;
	std::vector<Expression> arguments;

	const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const {
		return (const ExpressionBody&) *this;		
	};
};
