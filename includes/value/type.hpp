#pragma once
#include "types.h"

#include <cstdint>

enum class ValueType : uint8_t
{
#define t(T, rt, field, str, lfn) T,
	TYPE_LIST(t)
#undef t
};

template <typename T> struct TypeTraits;
#define trait_spec(T, rt, field, str, lfn)                                \
	template <> struct TypeTraits<rt>                                     \
	{                                                                     \
		static constexpr ValueType kind = ValueType::T;                   \
	};

TYPE_LIST(trait_spec)
#undef trait_spec

static const char* vtypes_names[] = // NOLINT
	{
#define t(T, rt, field, str, lfn) str,
		TYPE_LIST(t)
#undef t
};
