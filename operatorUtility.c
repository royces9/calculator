#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include "types.h"
#include "file.h"
#include "functions.h"
#include "operator.h"
#include "multi.h"
#include "operatorUtility.h"

//struct oper const empty_op = {.order = 0, .argNo = 0, .fp = NULL, .mat_op = 0, ._enum = 0};
#define empty_op {.order = 0, .argNo = 0, .fp.p = NULL, .mat_op = 0, ._enum = 0}

char const *const FUNCTION_LIST[FUNCTION_COUNT + 1] = {
	[eQuit] = "quit",
	[eClear] = "clear",
	[eList] = "list",
	[eHelp] = "help",

	[ePi] = "pi",
	[eE] = "e",
	[eAns] = "ans",

	[eSin] = "sin(",
	[eCos] = "cos(",
	[eTan] = "tan(",
	[eLn] = "ln(",
	[eLog] = "log(",

	[eSqrt] = "sqrt(",

	[eAsin] = "asin(",
	[eAcos] = "acos(",
	[eAtan] = "atan(",

	[eFloor] = "floor(",
	[eCeil] = "ceil(",
	[eRound] = "round(",
	[eMin] = "min(",
	[eMax] = "max(",
	[eAvg] = "avg(",
	[eSum] = "sum(",

	[eFactorial] = "factorial(",

	[eDeri] = "derivative(",
	[eInte] = "integral(",
	[eSolve] = "solve(",

	[eZeros] = "zeros(",
	[eOnes] = "ones(",
	[eEye] = "eye(",
	[eRand] = "rand(",
	[eSize] = "size(",

	[eNumel] = "numel(",
	[eMagnitude] = "magnitude(",

	[eTranspose] = "transpose(",
	[eLinspace] = "linspace(",

	[eRun] = "run(",
	[ePrint] = "print(",
	NULL
};


char const *const OPERATOR_LIST[OPERATOR_COUNT + 1] = {
	[eAdd] = "+",
	[eSubtract] = "-",
	[eMultiply] = ".*",
	[eDivide] = "./",

	[eExponent] = ".^",
	[eAssign] = "=",
	[eLeftParen] = "(",
	[eRightParen] = ")",

	[eMultiplyMatrix] = "*",
	[eDivideMatrix] = "/",
	[eExponentMatrix] = "^",
	[eModulo] = "%",

	[eLess] = "<",
	[eGreater] = ">",
	[eLessEqual] = "<=",
	[eGreaterEqual] = ">=",

	[eNotEqual] = "!=",
	[eEqual] = "==",

	[eAnd] = "&&",
	[eOr] = "||",
	[eNot] = "~",

	[eReference] = "r",
	NULL
};


struct oper
F_STRUCT[FUNCTION_COUNT + 1] = {
	[eQuit] = empty_op,
	[eClear] = empty_op,
	[eList] = empty_op,
	[eHelp] = empty_op,

	[ePi] = empty_op,
	[eE] = empty_op,
	[eAns] = empty_op,

	[eSin] = {.order = 15, .argNo = 1, .fp.s_one = &_sin, .mat_op = 0, ._enum = eSin},
	[eCos] = {.order = 15, .argNo = 1, .fp.s_one = &_cos, .mat_op = 0, ._enum = eCos},
	[eTan] = {.order = 15, .argNo = 1, .fp.s_one = &_tan, .mat_op = 0, ._enum = eTan},
	[eLn] = {.order = 15, .argNo = 1, .fp.s_one = &_log, .mat_op = 0, ._enum = eLn},
	[eLog] = {.order = 15, .argNo = 1, .fp.s_one = &_log10, .mat_op = 0, ._enum = eLog},

	[eSqrt] = {.order = 15, .argNo = 1, .fp.s_one = &_sqrt, .mat_op = 0, ._enum = eSqrt},

	[eAsin] = {.order = 15, .argNo = 1, .fp.s_one = &_asin, .mat_op = 0, ._enum = eAsin},
	[eAcos] = {.order = 15, .argNo = 1, .fp.s_one = &_acos, .mat_op = 0, ._enum = eAcos},
	[eAtan] = {.order = 15, .argNo = 1, .fp.s_one = &_atan, .mat_op = 0, ._enum = eAtan},

	[eFloor] = {.order = 15, .argNo = 1, .fp.s_one = &_floor, .mat_op = 0, ._enum = eFloor},
	[eCeil] = {.order = 15, .argNo = 1, .fp.s_one = &_ceil, .mat_op = 0, ._enum = eCeil},
	[eRound] = {.order = 15, .argNo = 1, .fp.s_one = &_round, .mat_op = 0, ._enum = eRound},
	[eMin] = {.order = 15, .argNo = 1, .fp.m_one = &min, .mat_op = 1, ._enum = eMin},
	[eMax] = {.order = 15, .argNo = 1, .fp.m_one = &max, .mat_op = 1, ._enum = eMax},
	[eAvg] = {.order = 15, .argNo = 1, .fp.m_one = &avg, .mat_op = 1, ._enum = eAvg},
	[eSum] = {.order = 15, .argNo = 1, .fp.m_one = &sum, .mat_op = 1, ._enum = eSum},

	[eFactorial] = {.order = 15, .argNo = 1, .fp.s_one = &factorial, .mat_op = 0, ._enum = eFactorial},

	[eDeri] = {.order = 15, .argNo = 1, .fp.p = NULL, .mat_op = 0, ._enum = eDeri},
	[eInte] = {.order = 15, .argNo = 1, .fp.p = NULL, .mat_op = 0, ._enum = eInte},
	[eSolve] = {.order = 15, .argNo = 1, .fp.p = NULL, .mat_op = 0, ._enum = eSolve},

	[eZeros] = {.order = 15, .argNo = 1, .fp.mult = &zeros, .mat_op = 1, ._enum = eZeros},
	[eOnes] = {.order = 15, .argNo = 1, .fp.mult = &ones, .mat_op = 1, ._enum = eOnes},
	[eEye] = {.order = 15, .argNo = 1, .fp.m_one = &eye, .mat_op = 1, ._enum = eEye},
	[eRand] = {.order = 15, .argNo = 1, .fp.mult = &rand_mat, .mat_op = 1, ._enum = eRand},
	[eSize] = {.order = 15, .argNo = 1, .fp.m_one = &get_size, .mat_op = 1, ._enum = eSize},
	[eNumel] = {.order = 15, .argNo = 1, .fp.m_one = &numel, .mat_op = 1, ._enum = eNumel},
	[eMagnitude] = {.order = 15, .argNo = 1, .fp.m_one = &magnitude, .mat_op = 1, ._enum = eMagnitude},
						       
	[eTranspose] = {.order = 15, .argNo = 1, .fp.m_one = &t_mat, .mat_op = 1, ._enum = eTranspose},
	[eLinspace] = {.order = 15, .argNo = 1, .fp.mult = &linspace, .mat_op = 0, ._enum = eLinspace},
	[eRun] = {.order = 15, .argNo = 1, .fp.p = NULL, .mat_op = 0, ._enum = eRun},
	[ePrint] = {.order = 15, .argNo = 1, .fp.p = NULL, .mat_op = 0, ._enum = ePrint},
	empty_op
};

struct oper
O_STRUCT[OPERATOR_COUNT + 1] = {
	[eAdd] = {.order = 6, .argNo = 2, .fp.s_two = &add, .mat_op = 0, ._enum = eAdd},
	[eSubtract] = {.order = 6, .argNo = 2, .fp.s_two = &subtract, .mat_op = 0, ._enum = eSubtract},
	[eMultiply] = {.order = 5, .argNo = 2, .fp.s_two = &multiply, .mat_op = 0, ._enum = eMultiply},
	[eDivide] = {.order = 5, .argNo = 2, .fp.s_two = &divide, .mat_op = 0, ._enum = eDivide},

	[eExponent] = {.order = 4, .argNo = 2, .fp.s_two = &exponent, .mat_op = 0, ._enum = eExponent},
	[eAssign] = {.order = 16, .argNo = 3, .fp.p = NULL, .mat_op = eAssign, ._enum = eAssign},
	[eLeftParen] = {.order = 16, .argNo = 0, .fp.p = NULL, .mat_op = eLeftParen, ._enum = eLeftParen},
	[eRightParen] = {.order = 15, .argNo = 0, .fp.p = NULL, .mat_op = eRightParen, ._enum = eRightParen},

	[eMultiplyMatrix] = {.order = 5, .argNo = 2, .fp.m_two = &mult_mat, .mat_op = 1, ._enum = eMultiplyMatrix},
	[eDivideMatrix] = {.order = 5, .argNo = 2, .fp.m_two = &div_mat, .mat_op = 1, ._enum = eDivideMatrix},
	[eExponentMatrix] = {.order = 4, .argNo = 2, .fp.m_two = &exp_mat, .mat_op = 1, ._enum = eExponentMatrix},
	[eModulo] = {.order = 5, .argNo = 2, .fp.s_two = &modulo, .mat_op = 0, ._enum = eModulo},

	[eLess] = {.order = 8, .argNo = 2, .fp.s_two = &less, .mat_op = 0, ._enum = eLess},
	[eGreater] = {.order = 8, .argNo = 2, .fp.s_two = &greater, .mat_op = 0, ._enum = eGreater},
	[eLessEqual] = {.order = 8, .argNo = 2, .fp.s_two = &less_equal, .mat_op = 0, ._enum = eLessEqual},
	[eGreaterEqual] = {.order = 8, .argNo = 2, .fp.s_two = &greater_equal, .mat_op = 0, ._enum = eGreaterEqual},

	[eNotEqual] = {.order = 9, .argNo = 2, .fp.s_two = &not_equal, .mat_op = 0, ._enum = eNotEqual},
	[eEqual] = {.order = 9, .argNo = 2, .fp.s_two = &equal, .mat_op = 0, ._enum = eEqual},

	[eAnd] = {.order = 13, .argNo = 2, .fp.s_two = &and, .mat_op = 0, ._enum = eAnd},
	[eOr] = {.order = 14, .argNo = 2, .fp.s_two = &or, .mat_op = 0, ._enum = eOr},
	[eNot] = empty_op,

	[eReference] = {.order = 0, .argNo = 2, .fp.m_two = &reference, .mat_op = 1, ._enum = eReference},
};
