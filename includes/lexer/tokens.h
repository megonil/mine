#ifndef token_list
#define token_list
#include "keywords.h"

#define TOKEN_LIST(T)                                                     \
	KEYWORD_LIST(T)                                                       \
	T(DoubleDot, "..")                                                    \
	T(IDiv, "//")                                                         \
	T(ThinArrow, "->")                                                    \
	T(FatArrow, "=>")                                                     \
	T(Ge, ">=")                                                           \
	T(Le, "<=")                                                           \
	T(Eq, "==")                                                           \
	T(Or, "||")                                                           \
	T(And, "&&")                                                          \
	T(NEq, "!=")                                                          \
	T(Type, "<type>")                                                     \
	T(Name, "<name>")                                                     \
	T(Attribute, "<attribute>")                                           \
	T(Literal, "<literal>")                                               \
	T(Eof, "EOF")

#endif // !token_list
