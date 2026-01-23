#include "ast/ast.h"

#include <compiler.hpp>

llvm::Value*
Compiler::VisitLiteralExpr(LiteralExpr* expr)
{
	switch (expr->GetValue().type) {}
}
