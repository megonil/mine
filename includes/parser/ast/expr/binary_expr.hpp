#pragma once
#include "ast/node.hpp"
#include "expr.hpp"
#include "token.hpp"

#include <cstdint>
#include <llvm/Support/SMLoc.h>

enum class BinOpr : uint8_t
{
	Wrong,

	Add,
	Sub,
	Mul,
	Div,
	Mod,
	IDiv,
	Pow,
};

class BinaryExpr : public Expr
{
	ExprP  left;
	ExprP  right;
	BinOpr opr;

public:
	explicit BinaryExpr(ExprP left, ExprP right, BinOpr opr,
						llvm::SMLoc start, llvm::SMLoc end)
		: Expr(NBinaryExpr, start, end),
		  left(std::move(left)),
		  right(std::move(right)),
		  opr(opr)
	{
	}

	static BinOpr
	TTypeToBinOpr(TokenType ttype)
	{
#define opr(o) return BinOpr::o;
		switch (static_cast<int>(ttype))
		{
		case '+': opr(Add);
		case '-': opr(Sub);
		case '*': opr(Mul);
		case '/': opr(Div);
		case '%': opr(Mod);
		case static_cast<int>(TokenType::IDiv): opr(IDiv);
		case '^': opr(Pow);
		default: opr(Wrong);
		}
	}
#undef opr

	static const char*
	BinOpr2str(BinOpr opr)
	{
		switch (opr)
		{
		case BinOpr::Wrong: return "wrong";
		case BinOpr::Add: return "+";
		case BinOpr::Sub: return "-";
		case BinOpr::Mul: return "*";
		case BinOpr::Div: return "/";
		case BinOpr::Mod: return "%";
		case BinOpr::IDiv: return "//";
		case BinOpr::Pow: return "^";
		}
	}

	BinOpr
	Opr() const
	{
		return opr;
	}

	Expr*
	Left() const
	{
		return left.get();
	}

	Expr*
	Right() const
	{
		return right.get();
	}

	constexpr static bool
	classof(const Node* node) // NOLINT(readability-identifier-naming)
	{
		return node->Kind() == NBinaryExpr;
	}
};
