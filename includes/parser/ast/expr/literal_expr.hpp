#include "ast/node.hpp"
#include "expr.hpp"
#include "value.hpp"

#include <llvm/IR/Type.h>
#include <llvm/Support/SMLoc.h>

class LiteralExpr : public Expr
{
	Value val;

public:
	explicit LiteralExpr(Value value, llvm::SMLoc start, llvm::SMLoc end)
		: Expr(NodeKind::NLiteralExpr, start, end), val(value)
	{
	}

	Value
	GetValue() const
	{
		return val;
	}

	void
	SetValue(struct Value new_val)
	{
		val = new_val;
	}

	ValueType
	Type() const
	{
		return val.type;
	}

	constexpr static bool
	classof(const Node* node) // NOLINT(readability-identifier-naming)
	{
		return node->Kind() == NLiteralExpr;
	}
};
