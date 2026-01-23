#define STMT_LIST(T) T(ExprStmt, "ExprStmt")

#define EXPR_LIST(T)                                                      \
	T(BinaryExpr, "BinaryExpr")                                           \
	T(LiteralExpr, "LiteralExpr")                                         \
	T(UnaryExpr, "UnaryExpr")

#define NODE_LIST(T)                                                      \
	STMT_LIST(T)                                                          \
	EXPR_LIST(T)
