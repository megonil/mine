#pragma once
#include "ast/ast.h"
#include "lexer.hpp"

#include <cstdint>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>
#include <map>
#include <print>
#include <token.hpp>
#include <vector>

#define DEBUG_PRINT

enum class PrecedenceLevel : uint8_t
{
	None,
	Assign,	 // =
	Or,		 // ||
	And,	 // &&
	Eq,		 // == !=
	Compare, // < > >= <=
	Term,	 // + -
	Factor,	 // / * %
	Unary,	 // unary operators
	Call,	 // function()
	Primary, // literals
};

class Parser;

using PrefixFn = ExprP (Parser::*)();
using InfixFn  = ExprP (Parser::*)(ExprP left);

struct Precedence
{
	PrecedenceLevel level;
	InfixFn			infix;
	PrefixFn		prefix;
};

#define rule(t, infixfn, prefixfn, plevel)                                \
	{                                                                     \
		static_cast<int>(t),                                              \
		{                                                                 \
			PrecedenceLevel::plevel, (infixfn), (prefixfn)                \
		}                                                                 \
	}

#define emptyrule(t) rule(t, nullptr, nullptr, PrecedenceLevel::None)

using RuleMap = std::map<int, Precedence>;
using AST	  = std::vector<StmtP>;

class Parser
{
	Lexer* lexer;
	Token  curr;
	Token  prev;

public:
	explicit Parser(Lexer* lexer) : lexer(lexer)
	{
	}
	void
	Parse(AST& tree);

	void
	DebugPrint(std::vector<StmtP>&);

private:
	StmtP
	stmt();

	StmtP
	expr_stmt();

	// exprs
	/// Precedence
	constexpr ExprP
	expr()
	{
		return prec(PrecedenceLevel::Assign);
	}

	ExprP
	prec(PrecedenceLevel from);

	ExprP
	binary(ExprP left);

	ExprP
	unary();

	ExprP
	literal();

	void
	next();

	static constexpr Precedence empty_prec = {.level =
												  PrecedenceLevel::None,
											  .infix  = nullptr,
											  .prefix = nullptr};

	static constexpr Precedence
	get_rule(TokenType ttype)
	{
		auto key = static_cast<int>(ttype);
		if (rules.contains(key))
		{
			return rules.at(key);
		}

		return empty_prec;
	}

	static const RuleMap rules;

	static constexpr llvm::StringRef
	tokenref(llvm::SMLoc start, llvm::SMLoc end)
	{
		llvm::StringRef ref = {
			start.getPointer(),
			static_cast<size_t>(end.getPointer() - start.getPointer())};

		std::print("Some ref: {}\n", ref.str());
		return ref;
	}
};
