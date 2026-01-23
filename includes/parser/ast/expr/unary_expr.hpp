#pragma once
#include "ast/node.hpp"
#include "expr.hpp"
#include "token.hpp"

#include <cstdint>
#include <llvm/Support/SMLoc.h>

enum class UnOpr : uint8_t
{
	Wrong,
	Negate,
	Not, // todo
};

class UnaryExpr : public Expr
{
	ExprP expr;
	UnOpr opr;

public:
	explicit UnaryExpr(UnOpr opr, ExprP expr, llvm::SMLoc start,
					   llvm::SMLoc end)
		: Expr(NUnaryExpr, start, end), expr(std::move(expr)), opr(opr)
	{
	}

	UnOpr
	Opr() const
	{
		return opr;
	}

	Expr*
	GetExpr() const
	{
		return expr.get();
	}

	constexpr static bool
	classof(const Node* node) // NOLINT(readability-identifier-naming)
	{
		return node->Kind() == NUnaryExpr;
	}

	static UnOpr
	TTypeToUnOpr(const TokenType type)
	{
		switch (static_cast<int>(type))
		{
		case '-': return UnOpr::Negate;
		case '!': return UnOpr::Not;
		default: return UnOpr::Wrong;
		}
	}

	static const char*
	UnOpr2str(UnOpr opr)
	{
		switch (opr)
		{
		case UnOpr::Wrong: return "wrong";
		case UnOpr::Negate: return "-";
		case UnOpr::Not: return "!";
		default: return "unknown";
		}
	}
};
