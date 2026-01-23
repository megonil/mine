#pragma once
#include "ast/ast.h"
#include "ast/expr/unary_expr.hpp"

#include <ast/visitor.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>

class Compiler : public Visitor<Compiler, llvm::Value*>
{
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder;
	size_t			  blocks_deep = 0;

public:
	explicit Compiler(AST* tree, std::string_view filename)
		: Visitor(tree), builder(context)
	{
	}

	llvm::Value*
	VisitLiteralExpr(LiteralExpr*);

	llvm::Value*
	VisitBinaryExpr(BinaryExpr*);

	llvm::Value*
	VisitUnaryExpr(UnaryExpr*);

	llvm::Value*
	VisitExprStmt(ExprStmt*);
};
