#pragma once
#include <error.hpp>
#include <llvm/Support/SourceMgr.h>
#include <token.hpp>

class Lexer
{
	llvm::SourceMgr* src_mgr;
	std::string		 buffer;
	unsigned		 fbuffer;
	const char*		 c;
	const char*		 start;

public:
	explicit Lexer(llvm::SourceMgr* mgr)
		: src_mgr(mgr), fbuffer(src_mgr->getMainFileID())
	{
		const auto* buf = src_mgr->getMemoryBuffer(fbuffer);
		start			= buf->getBufferStart();
		c				= start;
	}

	/// Lex 1 token
	Token
	Lex();

private:
	// main functions
	/// Recognises current buffer as a keyword or instead as an
	/// identificator
	void
	ident(Token&);

	/// Tokenize type(TokenType::Type is a special type)
	void
	type(Token&);

	/// Tokenize number
	void
	number(Token&);

	/// Tokenize string
	void
	string(Token&);

	/// Tokenize char
	void
	ch(Token&);

	// utilities

	/// Returns if *c == check_for
	bool
	check_next(char check_for);

	bool
	current_is_new_line();

	/// Move the c pointer
	void
	advance();

	void
	read_escape();

	constexpr unsigned char
	to_uchar();

	constexpr TokenType
	to_ttype();

	constexpr bool
	isalpha();

	constexpr bool
	isdigit();
};
