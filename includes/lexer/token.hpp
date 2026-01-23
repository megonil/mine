#pragma once
#include "semtypes.h"
#include "tokens.h"

#include <climits>
#include <cstdint>
#include <cstdio>
#include <format>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>
#include <string>
#include <type.hpp>

enum SemanticValueType : uint8_t
{
#define t(type, str) type,
	SEMTYPES_LIST(t)
#undef t
};

constexpr const char* ttsem_names[] = // NOLINT
	{
#define t(type, str) str,
		SEMTYPES_LIST(t)
#undef t
};

union SemanticValue
{
	int64_t			i;
	uint64_t		u;
	double			d;
	llvm::StringRef s;

	SemanticValue() : i(0)
	{
	}

	explicit SemanticValue(int64_t val) : i(val)
	{
	}

	explicit SemanticValue(uint64_t val) : u(val)
	{
	}

	explicit SemanticValue(double val) : d(val)
	{
	}

	explicit SemanticValue(llvm::StringRef val) : s(val)
	{
	}
};

struct SemanticInfo
{
	SemanticValue	  value;
	SemanticValueType type = SemanticValueType::None;

	SemanticInfo() = default;

	SemanticInfo(int64_t val, SemanticValueType kind)
		: value(val), type(kind)
	{
	}

	SemanticInfo(uint64_t val, SemanticValueType kind)
		: value(val), type(kind)
	{
	}

	SemanticInfo(double val, SemanticValueType kind)
		: value(val), type(kind)
	{
	}

	void
	Set(int64_t val, SemanticValueType kind)
	{
		type	= kind;
		value.i = val;
	}

	void
	Set(uint64_t val, SemanticValueType kind)
	{
		type	= kind;
		value.u = val;
	}

	void
	Set(double val, SemanticValueType kind)
	{
		type	= kind;
		value.d = val;
	}

	void
	Set(llvm::StringRef val)
	{
		value.s = val;
		type	= StringVal;
	}

	llvm::StringRef
	GetString() const
	{
		return value.s;
	}

	int64_t
	GetInt() const
	{
		return value.i;
	}

	uint64_t
	GetUint() const
	{
		return value.u;
	}

	double
	GetDouble() const
	{
		return value.d;
	}

	const char*
	Type2str() const
	{
		return ttsem_names[type]; // NOLINT
	}
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
	SemanticInfo sem;
	TokenType	 kind = TokenType::Default;
	llvm::SMLoc	 start;
	llvm::SMLoc	 end;

	Token() = default;

	explicit Token(llvm::SMLoc start) : start(start)
	{
	}

	explicit Token(TokenType type, SemanticInfo& sem, llvm::SMLoc start,
				   llvm::SMLoc end)
		: sem(sem), kind(type), start(start), end(end)
	{
	}

	explicit Token(TokenType type, llvm::SMLoc start, llvm::SMLoc end)
		: kind(type), start(start), end(end)
	{
	}

	bool
	IsType() const
	{
		return kind == TokenType::Type;
	}

	bool
	IsLiteral() const
	{
		return kind == TokenType::Literal;
	}

	std::string
	String() const
	{
		if (sem.type != None)
		{
			std::string base = token_type2str(kind);

			base += '(';
			switch (sem.type)
			{
			case I8:
			case I16:
			case I32:
			case I64: base += std::to_string(sem.GetInt()); break;
			case U8:
			case U16:
			case U32:
			case U64: base += std::to_string(sem.GetUint()); break;
			case F32:
			case F64: base += std::format("{:g}", sem.GetDouble()); break;
			case Name:
			case StringVal: base += sem.GetString().str(); break;
			default: __builtin_unreachable();
			}

			base += ':';
			base += sem.Type2str();
			base += ')';

			return base;
		}

		return token_type2str(kind);
	}

	void
	SetEnd(llvm::SMLoc new_end)
	{
		end = new_end;
	}
};
