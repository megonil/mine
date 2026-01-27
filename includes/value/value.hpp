#pragma once
#include "token.hpp"
#include "type.hpp"
#include "types.h"

#include <cstdint>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Type.h>

union ValueData
{
#define t(T, rt, field, str, lfn) rt field;
	TYPE_LIST(t)
#undef t
};

struct Value
{
	llvm::StringRef repr;
	ValueData		data{};
	ValueType		type;

	template <typename T>
	Value(T val, llvm::StringRef ref)
		: repr(ref), type(TypeTraits<std::decay_t<T>>::kind)
	{
		set_value_unsafe<std::decay_t<T>>(val);
	}

	template <typename T>
	Value(T val, ValueType forced, llvm::StringRef ref)
		: repr(ref), type(forced)
	{
		set_value_generic(val, forced);
	}

	Value(SemanticInfo& info, llvm::StringRef ref)
		: repr(ref), type(static_cast<ValueType>(info.type))
	{
		switch (info.type)
		{
		case I8: data.i8 = static_cast<int8_t>(info.GetInt()); break;
		case I16: data.i16 = static_cast<int16_t>(info.GetInt()); break;
		case I32: data.i32 = static_cast<int32_t>(info.GetInt()); break;
		case I64: data.i64 = info.GetInt(); break;

		case U8: data.u8 = static_cast<uint8_t>(info.GetUint()); break;
		case U16: data.u16 = static_cast<uint16_t>(info.GetUint()); break;
		case U32: data.u32 = static_cast<uint32_t>(info.GetUint()); break;
		case U64: data.u64 = info.GetUint(); break;

		case F32: data.f32 = static_cast<float>(info.GetDouble()); break;
		case F64: data.f64 = info.GetDouble(); break;

		// TODO
		case Name:
		case StringVal:
		case None: break;
		}
	}

	std::string
	String() const
	{
		std::string res;
		if (repr.data() != nullptr)
		{
			res = repr.str();
		}

		res += ':';

		auto idx = static_cast<int>(type);
		if (idx >= 0 && idx < static_cast<int>(ValueType::F64))
		{
			res += vtypes_names[idx]; // NOLINT
		}
		else
		{
			res += "unknown type";
		}

		return res;
	}

	struct TypePair
	{
		llvm::Type* left;
		llvm::Type* right;
	};

	static llvm::Type*
	CommonType(TypePair pair, llvm::LLVMContext* context)
	{
		auto* left	= pair.left;
		auto* right = pair.right;
		if (left == right)
		{
			return left;
		}
		if (left->isIntegerTy() && right->isIntegerTy())
		{
			return left->getIntegerBitWidth() > right->getIntegerBitWidth()
					   ? left
					   : right; // pick bigger type
		}
		if (left->isDoubleTy() || right->isDoubleTy())
		{
			return llvm::Type::getDoubleTy(*context);
		}

		return llvm::Type::getFloatTy(*context);
	}

private:
	template <typename T>
	void
	set_value_unsafe(T val)
	{
#define set_direct(Ty, rt, field, str, lfn)                               \
	if constexpr (std::is_same_v<T, rt>)                                  \
	{                                                                     \
		data.field = val;                                                 \
		return;                                                           \
	}

		TYPE_LIST(set_direct)
#undef set_direct
	}

	template <typename T>
	void
	set_value_generic(T val, ValueType target_type)
	{
		switch (target_type)
		{
#define set_switch(T, rt, field, str, lfn)                                \
	case ValueType::T: data.field = static_cast<rt>(val); break;
			TYPE_LIST(set_switch)
		}
#undef set_switch
	}
};
