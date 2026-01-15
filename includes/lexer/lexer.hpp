#include <error.hpp>
#include <llvm/Support/SourceMgr.h>

class Lexer
{
	const llvm::SourceMgr* src_mgr_;
	ErrorMaker*			   errorer_;
	std::string			   buffer_;
};
