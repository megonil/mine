#pragma once

#include "ast/node.hpp"

#include <llvm/Support/SMLoc.h>
#include <memory>

class Expr : public Node
{
public:
	explicit Expr(NodeKind kind, llvm::SMLoc start, llvm::SMLoc end)
		: Node(kind, start, end)
	{
	}

	constexpr static bool
	classof(const Node* node) // NOLINT(readability-identifier-naming)
	{
		return node->Kind() > NExprsStart && node->Kind() < NExprsEnd;
	}
};

using ExprP = std::unique_ptr<Expr>;
