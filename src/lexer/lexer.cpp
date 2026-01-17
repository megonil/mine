#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <lexer.hpp>
#include <print>
#include <string>

#define advance() c++
#define save()                                                            \
	buffer += *c;                                                         \
	advance()

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

Token
Lexer::Lex() // NOLINT(readability-function-cognitive-complexity)
{
	while (std::isspace(to_uchar()) != 0)
	{
		advance();
	}

	Token result;
	buffer.clear();

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

	default:
		// keyword or name or number
		if (isdigit())
		{
			number(result);
		}
		else if (isalpha())
		{
			save();
			while (isalpha() || *c == '_')
			{
				save();
			}

			ident(result);
		}
		else
		{
			result.kind = static_cast<TokenType>(*c);
			advance();
		}
	}

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
				// placeholder
				std::println("Multiple dots");
				std::exit(1);
			}

			has_dot = true;
		}
		save();
	}

	llvm::StringRef ref(number_start, c - number_start);

	SemanticValueType val_type = has_dot ? F64 : F32;
#define suffix(c, t, check, msg)                                          \
	case c:                                                               \
		if (check)                                                        \
		{                                                                 \
			std::println(msg);                                            \
			std::exit(1);                                                 \
		}                                                                 \
		val_type = t;                                                     \
		break;

#define int_suffix(c, t)                                                  \
	suffix(c, t, has_dot, "Found integer suffix with a float")

#define float_suffix(c, t)                                                \
	suffix(c, t, !has_dot, "Found float suffix with an integer")

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
			std::println("Invalid number suffix");
			std::exit(1);
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

void
Lexer::string(Token& tok)
{
}

void
Lexer::ident(Token& tok)
{
}

void
Lexer::ch(Token& tok)
{
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
