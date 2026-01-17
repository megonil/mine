#pragma once
#include "keywords.h"

#include <error.hpp>
#include <llvm/Support/SourceMgr.h>
#include <token.hpp>
#include <unordered_map>

class Lexer
{
	llvm::SourceMgr*						   src_mgr;
	unsigned								   fbuffer;
	const char*								   c;
	const char*								   start;
	std::unordered_map<std::string, TokenType> keywords;

public:
	explicit Lexer(llvm::SourceMgr* mgr)
		: src_mgr(mgr), fbuffer(src_mgr->getMainFileID())
	{
		const auto* buf = src_mgr->getMemoryBuffer(fbuffer);
		start			= buf->getBufferStart();
		c				= start;
#define t(variant, str) {str, TokenType::variant},
		keywords = {KEYWORD_LIST(t)};
#undef t
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

	/// Move the c pointer
	void
	advance();

	char
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
