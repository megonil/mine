#include <llvm/Support/SMLoc.h>
#include <llvm/Support/SourceMgr.h>
#include <span>

#define max_arguments 16

enum class ErrorKind
{
};

class ErrorMaker
{
public:
	/// throw error at some loc
	static void
	Error(ErrorKind kind, llvm::SourceMgr* src, llvm::SMRange range,
		  llvm::SMLoc loc);

	/// just throw error
	static void
	JustError(ErrorKind kind, std::span<std::string, max_arguments> args);
};
