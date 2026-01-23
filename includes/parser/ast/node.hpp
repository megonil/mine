#pragma once

#include "nodes.h"

#include <cstdint>
#include <llvm/Support/SMLoc.h>

// clang-format off
enum NodeKind : uint8_t
{
#define t(Name, Str) N##Name,

	NStmtsStart, STMT_LIST(t) NStmtsEnd,
	NExprsStart, EXPR_LIST(t) NExprsEnd,

#undef t
};

static const char* nodes_names[] = // NOLINT
{
#define t(Name, Str) Str,

		NODE_LIST(t)

#undef t
};
// clang-format on

class Node
{
	NodeKind	kind;
	llvm::SMLoc start;
	llvm::SMLoc end;

public:
	Node(const Node&) = default;
	Node(Node&&)	  = default;
	Node&
	operator=(const Node&) = default;
	Node&
	operator=(Node&&) = default;
	Node(NodeKind kind, llvm::SMLoc start, llvm::SMLoc end)
		: kind(kind), start(start), end(end)
	{
	}
	~Node() = default;

	NodeKind
	Kind() const
	{
		return kind;
	}

	llvm::SMLoc
	Start()
	{
		return start;
	}

	llvm::SMLoc
	End()
	{
		return end;
	}

	const char*
	String()
	{
		return nodes_names // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
			[kind];
	}
};
