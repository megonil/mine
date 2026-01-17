#include "errors.h"

#include <cstdint>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/SourceMgr.h>

#define max_arguments 16

enum class ErrorKind : uint8_t
{
// lexer errors
#define t(variant, string) variant,
	ERROR_LIST(t)
#undef t
};

class ErrorMaker
{
public:
	/// throw error at some loc
	static void
	Error(ErrorKind kind, llvm::SourceMgr* src, llvm::SMRange range,
		  llvm::SMLoc loc);

	/// just throw error
	template <typename... Args>
	static void
	JustError(ErrorKind kind, Args&&... args)
	{
		std::string fmt = ErrorMaker::ErrKind2str(kind);
		std::string msg =
			llvm::formatv(fmt.c_str(), std::forward<Args>(args)...);

		llvm::errs() << llvm::raw_fd_ostream::RED
					 << "error: " << llvm::raw_fd_ostream::RESET << msg;
	}

	constexpr static const char*
	ErrKind2str(ErrorKind kind)
	{
		switch (kind)
		{
#define t(variant, string)                                                \
	case ErrorKind::variant: return string;
			ERROR_LIST(t)
#undef t
		}
	}
};
