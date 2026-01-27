#define TYPE_LIST(T)                                                      \
	T(I8, int8_t, i8, "i8", getInt8Ty)                                    \
	T(I16, int16_t, i16, "i16", getInt16Ty)                               \
	T(I32, int32_t, i32, "i32", getInt32Ty)                               \
	T(I64, int64_t, i64, "i64", getInt64Ty)                               \
	T(U8, uint8_t, u8, "u8", getInt8Ty)                                   \
	T(U16, uint16_t, u16, "u16", getInt16Ty)                              \
	T(U32, uint32_t, u32, "u32", getInt32Ty)                              \
	T(U64, uint64_t, u64, "u64", getInt64Ty)                              \
	T(F32, float, f32, "f32", getFloatTy)                                 \
	T(F64, double, f64, "f64", getDoubleTy)
