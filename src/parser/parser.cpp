#include "ast/ast.h"

#include <iostream>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>
#include <memory>
#include <parser.hpp>

#define none nullptr
#define pbinary &Parser::binary
#define punary &Parser::unary
#define pliteral &Parser::literal

// clang-format off
const RuleMap Parser::rules = // NOLINT(cert-err58-cpp)
{
//     ┌───────────────────────────────────────────────────────────────────────────────────────────┐
//     │ rule │ For Token          │ infix function  │ prefix function  │ precedence level         │
//     └───────────────────────────────────────────────────────────────────────────────────────────┘
	 rule ( '+'                , pbinary         , none             , Term                    ),
  	 rule ( '-'                , pbinary         , punary           , Term                    ),
	 rule ( '*'                , pbinary         , none             , Factor                  ),
	 rule ( '/'                , pbinary         , none             , Factor                  ),

	 rule ( TokenType::Eq      , pbinary         , none             , Eq                      ),
	 rule ( TokenType::And     , pbinary         , none             , And                     ),
	 rule ( TokenType::Or      , pbinary         , none             , Or                      ),

	 rule ( TokenType::Literal , none            , pliteral         , None                    ),
	 rule ( TokenType::Name    , none            , pliteral         , None                    ),
//     │      │                    │                 │                  │                          │
//     └──────┴────────────────────┴─────────────────┴──────────────────┴──────────────────────────┘
};
// clang-format on
#undef none
#undef bin
#undef unary
#undef literal

// placeholder
#define error(msg)                                                        \
	std::cerr << (msg) << '\n';                                           \
	std::exit(1);

void
Parser::next()
{
	prev = curr;
	curr = lexer->Lex();
#ifdef DEBUG_PRINT
	std::cout << curr.String() << '\n';
#endif
}

void
Parser::Parse(AST& tree)
{
	next(); // Init

	while (curr.kind != TokenType::Eof)
	{
		tree.push_back(stmt());
	}
}

StmtP
Parser::stmt()
{
	switch (curr.kind)
	{
	default: return expr_stmt(); // For now it's just expr stmts
	}
}

StmtP
Parser::expr_stmt()
{
	return std::make_unique<ExprStmt>(expr());
}

ExprP
Parser::prec(PrecedenceLevel from)
{
	Precedence prec	  = get_rule(curr.kind);
	PrefixFn   prefix = prec.prefix;

	if (prec.prefix == nullptr)
	{
		error("Expected expression")
	}

	ExprP left = (this->*prefix)();

	while (from < get_rule(curr.kind).level)
	{
		InfixFn infix = get_rule(curr.kind).infix;
		if (infix == nullptr)
		{
			break;
		}

		next();

		left = (this->*infix)(std::move(left));
	}

	return left;
}

ExprP
Parser::binary(ExprP left)
{
	// current is an expr
	// previous is an operator
	auto opr = BinaryExpr::TTypeToBinOpr(prev.kind);
	if (opr == BinOpr::Wrong)
	{
		error("Expected binary operator")
	}

	auto level = get_rule(prev.kind).level;

	auto right = prec(level);

	return std::make_unique<BinaryExpr>(std::move(left), std::move(right),
										opr, left->Start(), right->End());
}

ExprP
Parser::literal()
{
	if (!curr.IsLiteral())
	{
		error("Expected literal");
	}

	llvm::SMLoc start = curr.start;
	llvm::SMLoc end	  = curr.end;

	Value val(curr.sem, tokenref(start, end));
	next();

	return std::make_unique<LiteralExpr>(val, start, end);
}

ExprP
Parser::unary()
{
	return nullptr;
}
#undef error
