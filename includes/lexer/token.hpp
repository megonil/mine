#include <climits>
#include <cstdint>

enum class Token : uint16_t
{
	TK_CRAFT = UCHAR_MAX, // var declaration
	TK_ALEX,
};
