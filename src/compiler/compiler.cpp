#include "ast/ast.h"
#include "ast/expr/binary_expr.hpp"
#include "ast/expr/unary_expr.hpp"

#include <compiler.hpp>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Instruction.h>
#include <stdexcept>
#include <type_traits>

llvm::Value*
Compiler::VisitLiteralExpr(LiteralExpr* expr)
{
	auto val = expr->GetValue();
	switch (val.type)
	{
#define t(T, rt, field, str, lfn)                                         \
	case ValueType::T:                                                    \
		if constexpr (std::is_floating_point_v<rt>)                       \
		{                                                                 \
			return llvm::ConstantFP::get(                                 \
				llvm::Type::lfn(context),                                 \
				static_cast<double>(val.data.field));                     \
		}                                                                 \
		else                                                              \
		{                                                                 \
			return llvm::ConstantInt::get(                                \
				llvm::Type::lfn(context),                                 \
				static_cast<uint64_t>(val.data.field),                    \
				std::is_signed_v<rt>);                                    \
		}

		TYPE_LIST(t)

#undef t
	}
}

llvm::Value*
Compiler::VisitUnaryExpr(UnaryExpr* expr)
{
	auto* val = Visit(expr->GetExpr());
	switch (expr->Opr())
	{
	case UnOpr::Negate: return builder.CreateNeg(val);
	case UnOpr::Not: return builder.CreateNot(val);
	default: __builtin_unreachable();
	}
}

llvm::Value*
Compiler::VisitBinaryExpr(BinaryExpr* expr)
{
	auto* left		  = Visit(expr->Left());
	auto* right		  = Visit(expr->Right());
	auto* common_type = Value::CommonType(
		{.left = left->getType(), .right = right->getType()}, &context);

	left = create_cast(left, common_type);

	llvm::Instruction::BinaryOps opcode = llvm::Instruction::FAdd;
	bool is_float						= common_type->isFloatingPointTy();
#define setop(iffloat, els)                                               \
	opcode =                                                              \
		is_float ? llvm::Instruction::iffloat : llvm::Instruction::els;   \
	break;
	switch (expr->Opr())
	{
	case BinOpr::Add: setop(FAdd, Add);
	case BinOpr::Sub: setop(FSub, Sub);
	case BinOpr::Mul: setop(FMul, Mul);
	case BinOpr::Div: setop(FDiv, UDiv);
	case BinOpr::Mod: setop(FRem, URem);

	// TODO:
	case BinOpr::IDiv:
	case BinOpr::Pow: break; // do nothing for now
	default: __builtin_unreachable();
	}

	return builder.CreateBinOp(opcode, left, right);
}

llvm::Value*
Compiler::create_cast(llvm::Value* val, llvm::Type* dest)
{
	llvm::Type* source = val->getType();

	if (source == dest)
	{
		return val;
	}

	if (source->isIntegerTy() && dest->isIntegerTy())
	{
		return builder.CreateSExtOrTrunc(val, dest);
	}
	if (source->isIntegerTy() && dest->isFloatingPointTy())
	{
		return builder.CreateSIToFP(val, dest);
	}

	if (source->isFloatingPointTy() && dest->isFloatingPointTy())
	{
		return builder.CreateFPTrunc(val, dest);
	}

	throw std::runtime_error("Unknown cast");
}

llvm::Value*
Compiler::VisitExprStmt(ExprStmt* stmt)
{
	return Visit(stmt->GetExpr());
}

void
Compiler::implicit_main(llvm::Value* last)
{
	using namespace llvm;
	std::vector<llvm::Type*> args;

	llvm::FunctionType* ret_type = llvm::FunctionType::get(
		llvm::Type::getInt32Ty(context), args, false);
	llvm::Function* fun = llvm::Function::Create(
		ret_type, llvm::GlobalValue::ExternalLinkage, "main",
		module.get());

	llvm::BasicBlock* entry =
		llvm::BasicBlock::Create(context, "entry", fun);
	builder.SetInsertPoint(entry);
	builder.CreateRet(last);
}
