#pragma once
#include "ast/expr/expr.hpp"
#include "stmt.hpp"

class ExprStmt : public Stmt
{
	ExprP expression;

public:
	explicit ExprStmt(ExprP expr)
		: Stmt(NodeKind::NExprStmt, expr->Start(), expr->End()),
		  expression(std::move(expr))
	{
	}

	Expr*
	GetExpr()
	{
		return expression.get();
	}

	constexpr static bool
	classof(const Node* node) // NOLINT(readability-identifier-naming)
	{
		return node->Kind() == NExprStmt;
	}
};
