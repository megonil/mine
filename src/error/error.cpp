#include "error.hpp"

void
ErrorMaker::Error(ErrorKind kind, llvm::SourceMgr* src,
				  llvm::SMRange range, llvm::SMLoc loc)
{
	std::string msg = ErrorMaker::ErrKind2str(kind);

	src->PrintMessage(loc, llvm::SourceMgr::DK_Error, msg, range);
	std::exit(1);
}
