#pragma once
#include "ast/ast.h"
#include "ast/expr/unary_expr.hpp"

#include <ast/visitor.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>

class Compiler : public Visitor<Compiler, llvm::Value*>
{
	llvm::LLVMContext			  context;
	llvm::IRBuilder<>			  builder;
	std::unique_ptr<llvm::Module> module;
	size_t						  blocks_deep = 0;

	llvm::Value*
	create_cast(llvm::Value* val, llvm::Type* dest);

	void
	implicit_main(llvm::Value* last);

public:
	explicit Compiler(AST* tree, std::string_view filename)
		: Visitor(tree),
		  builder(context),
		  module(std::make_unique<llvm::Module>(filename, context))
	{
	}

	std::unique_ptr<llvm::Module>
	Module()
	{
		return std::move(module);
	}

	void
	VisitAll()
	{
		llvm::Value* val = nullptr;
		for (const auto& node : *tree)
		{
			val = Visit(node.get());
		}
		if (val != nullptr)
		{
			implicit_main(val);
		}
		else
		{
			implicit_main(llvm::ConstantInt::get(
				llvm::Type::getInt32Ty(context), 0));
		}
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
