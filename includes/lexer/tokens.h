#pragma once
#ifndef token_list
#define token_list

#define TOKEN_LIST(T)                                                     \
	T(Craft, "craft")                                                     \
	T(Mine, "mine")                                                       \
	T(Bedrock, "bedrock")                                                 \
	T(Forever, "forever")                                                 \
	T(Kit, "kit")                                                         \
	T(Trade, "trade")                                                     \
	T(Quest, "quest")                                                     \
	T(Block, "block")                                                     \
	T(Overworld, "overworld")                                             \
	T(Nether, "nether")                                                   \
	T(Cube, "cube")                                                       \
	T(Loot, "loot")                                                       \
	T(Grind, "grind")                                                     \
	T(If, "if")                                                           \
	T(Else, "else")                                                       \
	T(Take, "take")                                                       \
	T(From, "from")                                                       \
	T(DoubleDot, "..")                                                    \
	T(ThinArrow, "->")                                                    \
	T(FatArrow, "=>")                                                     \
	T(Ge, ">=")                                                           \
	T(Le, "<=")                                                           \
	T(Eq, "==")                                                           \
	T(Or, "||")                                                           \
	T(And, "&&")                                                          \
	T(NEq, "!=")                                                          \
	T(Type, "type")                                                       \
	T(Name, "<name>")                                                     \
	T(Attribute, "<attribute>")                                           \
	T(Literal, "<literal>")                                               \
	T(Eof, "EOF")

#endif // !token_list
