#ifndef mine_semtypes
#define mine_semtypes

#define SEMTYPES_LIST(T)                                                  \
	T(I8, "i8")                                                           \
	T(I16, "i16")                                                         \
	T(I32, "i32")                                                         \
	T(I64, "i64")                                                         \
	T(U8, "u8")                                                           \
	T(U16, "u16")                                                         \
	T(U32, "u32")                                                         \
	T(U64, "u64")                                                         \
	T(F32, "f32")                                                         \
	T(F64, "f64")                                                         \
	T(None, "none")                                                       \
	T(StringVal, "string")                                                \
	T(Name, "name")

#endif // !mine_semtypes
