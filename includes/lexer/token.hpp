#include "tokens.h"

#include <climits>
#include <cstdint>
#include <cstdio>

union SemanticInfo
{
	int			i;
	double		d;
	const char* s;
};

enum class TokenType : uint16_t
{
	// all default 1-char symbols represented by their ASCII value
	// keywords
	Default = UCHAR_MAX,

#define t(name, str) name,

	TOKEN_LIST(t)

#undef t
};

constexpr size_t tokens_count = static_cast<size_t>(TokenType::Eof) -
								static_cast<size_t>(TokenType::Default) +
								1;
#define t_idx(name)                                                       \
	[static_cast<size_t>(TokenType::name) -                               \
		static_cast<size_t>(TokenType::Default)]

constexpr const char* ttype_names[] = { // NOLINT
#define t(name, str) str,
	TOKEN_LIST(t)
#undef t
};

#define max_token_buff 16

inline const char*
token_type2str(TokenType token_type)
{
	static char string[max_token_buff] = {}; // NOLINT
	auto		val					   = static_cast<uint16_t>(token_type);

	if (val < UCHAR_MAX)
	{
		std::snprintf(string, max_token_buff, "%c", (char) val); // NOLINT
		return string;											 // NOLINT
	}

	uint16_t index = val - static_cast<uint16_t>(TokenType::Default) - 1;
	if (index < (sizeof(ttype_names) / sizeof(char*)))
	{
		return ttype_names[index]; // NOLINT
	}

	return "UNKNOWN";
}

struct Token
{
	TokenType	 kind = TokenType::Default;
	SemanticInfo sem{};

	Token() = default;

	explicit Token(SemanticInfo* sem) : sem(*sem)
	{
	}

	explicit Token(SemanticInfo sem, TokenType kind) : kind(kind), sem(sem)
	{
	}

	bool
	IsType() const
	{
		return kind == TokenType::Type;
	}

	const char*
	String() const
	{
		return token_type2str(kind);
	}
};
