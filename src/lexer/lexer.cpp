#pragma once
#include <cctype>
#include <lexer.hpp>

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
	while (std::isspace(static_cast<unsigned char>(*c)) != 0)
	{
		advance();
	}

	Token result;
	buffer.clear();

	switch (*c)
	{
	case EOF:
	case '\0':
		result.kind = TokenType::Eof;
		return result;

		doubletok('!', '=', TokenType::NEq);
		doubletok('=', '=', TokenType::Eq);
		doubletok('>', '=', TokenType::Ge);
		doubletok('<', '=', TokenType::Le);
		doubletok('|', '|', TokenType::Or);
		doubletok('&', '&', TokenType::And);

	default:
		// keyword or name or number
		if (std::isdigit(static_cast<unsigned char>(*c)) != 0)
		{
			number(result);
		}
		else if (std::isalpha(static_cast<unsigned char>(*c)) != 0)
		{
			save();
			while (std::isalpha(*c) != 0 || *c == '_')
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
}

void
Lexer::string(Token& tok)
{
}

void
Lexer::ident(Token& tok)
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

#undef save
#undef advance
