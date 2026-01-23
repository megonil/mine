#pragma once
#include "ast/node.hpp"

#include <memory>

class Stmt : public Node
{
public:
	explicit Stmt(NodeKind kind, llvm::SMLoc start, llvm::SMLoc end)
		: Node(kind, start, end)
	{
	}

	constexpr static bool
	classof(const Node* node) // NOLINT(readability-identifier-naming)
	{
		return node->Kind() > NStmtsStart && node->Kind() < NStmtsEnd;
	}
};

using StmtP = std::unique_ptr<Stmt>;
