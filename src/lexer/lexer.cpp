#include <cctype>
#include <cmath>
#include <lexer.hpp>
#include <llvm/Support/SMLoc.h>

#define advance() c++

#define doubletok(t, dt, double_t)                                        \
	case t:                                                               \
	{                                                                     \
		advance();                                                        \
		if (check_next(dt))                                               \
		{                                                                 \
			result.kind = double_t;                                       \
		}                                                                 \
		else                                                              \
		{                                                                 \
			result.kind = static_cast<TokenType>(t);                      \
		}                                                                 \
		break;                                                            \
	}

#define lex_error(start, kind)                                            \
	llvm::SMLoc	  error_loc = llvm::SMLoc::getFromPointer(c);             \
	llvm::SMRange range(llvm::SMLoc::getFromPointer(start), error_loc);   \
	ErrorMaker::Error(kind, src_mgr, range, error_loc);

Token
Lexer::Lex() // NOLINT(readability-function-cognitive-complexity)
{
	while (std::isspace(to_uchar()) != 0)
	{
		advance();
	}

	Token result(llvm::SMLoc::getFromPointer(c));

	switch (*c)
	{
	case EOF:
	case '\0': result.kind = TokenType::Eof; return result;
	case '\'': ch(result); break;
	case '"':
		string(result);
		break;

		doubletok('!', '=', TokenType::NEq);
		doubletok('=', '=', TokenType::Eq);
		doubletok('>', '=', TokenType::Ge);
		doubletok('<', '=', TokenType::Le);
		doubletok('|', '|', TokenType::Or);
		doubletok('&', '&', TokenType::And);
		doubletok('/', '/', TokenType::IDiv);

	default:
		// keyword or name or number
		if (isdigit())
		{
			number(result);
		}
		else if (isalpha())
		{
			ident(result);
		}
		else
		{
			result.kind = to_ttype();
			advance();
		}
	}

	result.SetEnd(llvm::SMLoc::getFromPointer(c));
	return result;
}
#undef doubletok

void
Lexer::number(Token& tok)
{
	bool		has_dot		 = false;
	const char* number_start = c;

	while (isdigit() || *c == '.')
	{
		if (*c == '.')
		{
			if (has_dot)
			{
				lex_error(number_start, ErrorKind::MultipleDots);
			}

			has_dot = true;
		}

		advance();
	}
	tok.SetEnd(llvm::SMLoc::getFromPointer(c));

	llvm::StringRef ref(number_start, c - number_start);

	SemanticValueType val_type = has_dot ? F64 : I32;
#define suffix(c, t, check, kind)                                         \
	case c:                                                               \
		if (check)                                                        \
		{                                                                 \
			lex_error(number_start, kind)                                 \
		}                                                                 \
		val_type = t;                                                     \
		break;

#define int_suffix(c, t) suffix(c, t, has_dot, ErrorKind::InvalidIntSuffix)

#define float_suffix(c, t)                                                \
	suffix(c, t, !has_dot, ErrorKind::InvalidFloatSuffix)

	if (isalpha())
	{
		switch (*c)
		{
			int_suffix('s', I8);
			int_suffix('u', U32);
			float_suffix('f', F32);
			float_suffix('d', F64);

		default:
			// placeholder
			lex_error(number_start, ErrorKind::InvalidNumberSuffix);
		}

		advance(); // suffix
	}

#undef suffix
#undef int_suffix
#undef float_suffix

	if (has_dot)
	{
		double val = NAN;
		ref.getAsDouble(val);
		tok.kind = TokenType::Literal;
		tok.sem.Set(val, val_type);
	}
	else
	{
		constexpr int base = 10;
		int64_t		  val  = 0;
		ref.getAsInteger(base, val);
		tok.kind = TokenType::Literal;
		tok.sem.Set(val, val_type);
	}
}

char
Lexer::read_escape()
{
#define advbr()                                                           \
	advance();                                                            \
	break;

	char chr = 0; // result
	advance();	  // start of escape sequence

	switch (*c)
	{
	case 'a': chr = '\a'; advbr();
	case 'b': chr = '\b'; advbr();
	case 'f': chr = '\f'; advbr();
	case 'e': chr = '\e'; advbr();
	case 'r': chr = '\r'; advbr();
	case 'n': chr = '\n'; advbr();
	case 't': chr = '\t'; advbr();
	case 'v': chr = '\v'; advbr();
	case '\\':
	case '"':
	case '\'': chr = *c; advbr();

	case EOF: return chr;

	default: advance(); return chr;
	}

	return chr;
};

void
Lexer::string(Token& tok)
{
	const char* str_start = c;
	std::string val;
	advance(); // "

	while (*c != '"')
	{
		switch (*c)
		{
		case '\\': val += read_escape(); break;
		case '\0':
		case EOF:
		{
			lex_error(str_start, ErrorKind::UnfinishedString);
			break;
		}
		default: val += *c; advance();
		}
	}

	advance(); // "
	llvm::StringRef ref(val);
	tok.sem.Set(ref);
	tok.kind = TokenType::Literal;
	tok.SetEnd(llvm::SMLoc::getFromPointer(c));
}

void
Lexer::ident(Token& tok)
{
	const char* start = c;
	while (isalpha() || *c == '_')
	{
		advance();
	}

	llvm::StringRef ref(start, c - start);
	if (keywords.contains(ref.str()))
	{
		tok.kind = keywords[ref.str()];
		tok.SetEnd(llvm::SMLoc::getFromPointer(c));
		return;
	}

	tok.SetEnd(llvm::SMLoc::getFromPointer(c));
	tok.kind = TokenType::Name;
	tok.sem.Set(ref);
}

void
Lexer::ch(Token& tok)
{
	const char* chr_start = c;
	advance(); // '
	char chr = 0;

	if (*c == '\\')
	{
		chr = read_escape();
	}
	else
	{
		chr = *c;
	}
	if (*c != '\'')
	{
		lex_error(chr_start, ErrorKind::UnfinishedChar);
	}
	advance(); // ''

	tok.kind = TokenType::Literal;
	tok.sem.Set(static_cast<int64_t>(chr), I8);
	tok.SetEnd(llvm::SMLoc::getFromPointer(c));
}

bool
Lexer::check_next(char check_for)
{
	if (*c == check_for)
	{
		advance();
		return true;
	}

	return false;
}

constexpr unsigned char
Lexer::to_uchar()
{
	return static_cast<unsigned char>(*c);
}

constexpr TokenType
Lexer::to_ttype()
{
	return static_cast<TokenType>(to_uchar());
}

constexpr bool
Lexer::isalpha()
{
	return std::isalpha(to_uchar()) != 0;
}

constexpr bool
Lexer::isdigit()
{
	return std::isdigit(to_uchar()) != 0;
}

#undef save
#undef advance
