#pragma once

#include "ast.h"
#include "ast/node.hpp"
#include "ast/stmt/expr_stmt.hpp"
#include "parser.hpp"

#include <print>

#define vis(o) as_derived()->Visit##o(static_cast<o*>(node)) // NOLINT
template <typename Derived, typename RetType = void> class Visitor
{
	const AST* tree;

private:
	explicit Visitor(const AST* tree) : tree(tree)
	{
	}

public:
	void
	VisitAll()
	{
		std::println("Printer is working...");
		for (auto& node : *tree) // NOLINT(readability-qualified-auto)
		{
			Visit(node.get());
		}
	}

	RetType
	Visit(Node* node)
	{
		if (node == nullptr)
		{
			std::print("warning: Null node\n");
			std::exit(1);
		}
		switch (node->Kind())
		{
		case NExprStmt: return vis(ExprStmt);
		case NBinaryExpr: return vis(BinaryExpr);
		case NLiteralExpr: return vis(LiteralExpr);
		default:
			return as_derived()->VisitNode(node);
			// do nothing
		}
	}

	RetType
	VisitExprStmt(ExprStmt* stmt)
	{
		return as_derived()->VisitStmt(stmt);
	}

	RetType
	VisitLiteralExpr(LiteralExpr* expr)
	{ // Исправлен тип аргумента
		return as_derived()->VisitExpr(expr);
	}

	RetType
	VisitBinaryExpr(BinaryExpr* expr)
	{ // Исправлен тип аргумента
		return as_derived()->VisitExpr(expr);
	}

	RetType
	VisitExpr(Expr* expr)
	{
		return as_derived()->VisitNode(expr);
	}

	RetType
	VisitStmt(Stmt* stmt)
	{
		return as_derived()->VisitNode(stmt);
	}

	RetType
	VisitNode(Node* node)
	{
		return RetType();
	}

private:
	constexpr Derived*
	as_derived()
	{
		return static_cast<Derived*>(this);
	}

	friend Derived;
};

// NOLINTBEGIN(readability-convert-member-functions-to-static)
class ASTPrinter : public Visitor<ASTPrinter>
{
	int indent_level = 0;

	struct Indent
	{
		int* level;
		Indent(const Indent&) = delete;
		Indent(Indent&&)	  = delete;
		Indent&
		operator=(const Indent&) = delete;
		Indent&
		operator=(Indent&&) = delete;
		explicit Indent(int* level) : level(level)
		{
			*level += 2;
		}
		~Indent()
		{
			*level -= 2;
		}
	};

	void
	pad() const
	{
		std::print("{:>{}}", "", indent_level);
	}

public:
	explicit ASTPrinter(const AST* tree) : Visitor(tree)
	{
	}

	void
	VisitExprStmt(ExprStmt* stmt)
	{
		pad();
		std::print("ExprStmt(\n");
		{
			Indent scope(&indent_level);
			Visit(stmt->GetExpr());
		}
		std::print("\n");
		pad();
		std::print(")\n");
	}

	void
	VisitLiteralExpr(LiteralExpr* expr)
	{
		std::print("literal({})", expr->GetValue().String());
	}

	void
	VisitBinaryExpr(BinaryExpr* expr)
	{
		pad();
		std::print("Binary<{}>(\n", BinaryExpr::BinOpr2str(expr->Opr()));
		{
			Indent scope(&indent_level);

			pad();
			std::print("left: ");
			Visit(expr->Left());
			std::print("\n");
			pad();
			std::print("right: ");
			Visit(expr->Right());
			std::print("\n");
		}
		pad();
		std::print(")");
	}
};
// NOLINTEND(readability-convert-member-functions-to-static)
