#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include "types.h"
#include "file.h"
#include "functions.h"
#include "operator.h"
#include "multi.h"
#include "operatorUtility.h"

op_struct const empty_op = {.order = 0, .argNo = 0, .fp = NULL, .mat_op = 0, ._enum = 0};

char const *const FUNCTION_LIST[FUNCTION_COUNT + 1] = {
						       "quit",
						       "clear",
						       "list",
						       "help",

						       "pi",
						       "e",
						       "ans",

						       "sin(",
						       "cos(",
						       "tan(",
						       "ln(",
						       "log(",

						       "sqrt(",

						       "asin(",
						       "acos(",
						       "atan(",

						       "floor(",
						       "ceil(",
						       "round(",
						       "min(",
						       "max(",
						       "avg(",
						       "sum(",
  
						       "factorial(",

						       "derivative(",
						       "integral(",
						       "solve(",

						       "zeros(",
						       "ones(",
						       "eye(",
						       "rand(",
						       "size(",

						       "numel(",
						       "magnitude(",
	
						       "transpose(",
						       "linspace(",
  
						       "run(",
						       "print(",
						       NULL
};

void *const FUNCTION_POINTER[FUNCTION_COUNT + 1] = {
					      NULL,
					      NULL,
					      NULL,
					      NULL,

					      NULL,
					      NULL,
					      NULL,

					      &sin,
					      &cos,
					      &tan,
					      &log,
					      &log10,

					      &sqrt,

					      &asin,
					      &acos,
					      &atan,

					      &floor,
					      &ceil,
					      &round,
					      &min,
					      &max,
					      &avg,
					      &sum,

					      &factorial,

					      &deri,
					      &inte,
					      &solve,

					      &zeros,
					      &ones,
					      &eye,
					      &rand_mat,
					      &get_size,

					      &numel,
					      &magnitude,

					      &t_mat,
					      &linspace,

					      &runFile,
					      &printLine,
					      NULL
};

void *const OPERATOR_POINTER[OPERATOR_COUNT + 1] = {
						    &add,
						    &subtract,
						    &multiply,
						    &divide,

						    &exponent,
						    &assign,
						    NULL,
						    NULL,
						    
						    &mult_mat,
						    NULL,
						    &exp_mat,
						    &modulo,
						    
						    &less,
						    &greater,
						    &less_equal,
						    &greater_equal,

						    &not_equal,
						    &equal,

						    &and,
						    &or,
						    NULL,

						    &reference,
						    NULL
						    
};

char const *const OPERATOR_LIST[OPERATOR_COUNT + 1] = {
						       "+",
						       "-",
						       ".*",
						       "./",

						       ".^",
						       "=",
						       "(",
						       ")",

						       "*",
						       "/",
						       "^",
						       "%",

						       "<",
						       ">",
						       "<=",
						       ">=",

						       "!=",
						       "==",

						       "&&",
						       "||",
						       "~",

						       "r",
						       NULL
};


uint8_t const operatorPrecedence[OPERATOR_COUNT] = {
						    6,
						    6,
						    5,
						    5,

						    4,
						    4,
						    16,
						    15,

						    5,
						    5,
						    4,
						    5,
  
						    15,
						    8,
						    8,
						    8,

						    8,
						    9,

						    9,
						    11,
						    12,

						    0
}; //~is not implemented at the moment

op_struct
F_STRUCT[FUNCTION_COUNT + 1] = {
				[eQuit] = empty_op,
				[eClear] = empty_op,
				[eList] = empty_op,
				[eHelp] = empty_op,

				[ePi] = empty_op,
				[eE] = empty_op,
				[eAns] = empty_op,

				[eSin] = {.order = 15, .argNo = 1, .fp = &_sin, .mat_op = 0, ._enum = eSin},
				[eCos] = {.order = 15, .argNo = 1, .fp = &_cos, .mat_op = 0, ._enum = eCos},
				[eTan] = {.order = 15, .argNo = 1, .fp = &_tan, .mat_op = 0, ._enum = eTan},
				[eLn] = {.order = 15, .argNo = 1, .fp = &_log, .mat_op = 0, ._enum = eLn},
				[eLog] = {.order = 15, .argNo = 1, .fp = &_log10, .mat_op = 0, ._enum = eLog},

				[eSqrt] = {.order = 15, .argNo = 1, .fp = &_sqrt, .mat_op = 0, ._enum = eSqrt},

				[eAsin] = {.order = 15, .argNo = 1, .fp = &_asin, .mat_op = 0, ._enum = eAsin},
				[eAcos] = {.order = 15, .argNo = 1, .fp = &_acos, .mat_op = 0, ._enum = eAcos},
				[eAtan] = {.order = 15, .argNo = 1, .fp = &_atan, .mat_op = 0, ._enum = eAtan},

				[eFloor] = {.order = 15, .argNo = 1, .fp = &_floor, .mat_op = 0, ._enum = eFloor},
				[eCeil] = {.order = 15, .argNo = 1, .fp = &_ceil, .mat_op = 0, ._enum = eCeil},
				[eRound] = {.order = 15, .argNo = 1, .fp = &_round, .mat_op = 0, ._enum = eRound},
				[eMin] = {.order = 15, .argNo = 1, .fp = &min, .mat_op = 1, ._enum = eMin},
				[eMax] = {.order = 15, .argNo = 1, .fp = &max, .mat_op = 1, ._enum = eMax},
				[eAvg] = {.order = 15, .argNo = 1, .fp = &avg, .mat_op = 1, ._enum = eAvg},
				[eSum] = {.order = 15, .argNo = 1, .fp = &sum, .mat_op = 1, ._enum = eSum},

				[eFactorial] = {.order = 15, .argNo = 1, .fp = &factorial, .mat_op = 0, ._enum = eFactorial},

				[eDeri] = {.order = 15, .argNo = 1, .fp = &deri, .mat_op = 0, ._enum = eDeri},
				[eInte] = {.order = 15, .argNo = 1, .fp = &round, .mat_op = 0, ._enum = eInte},
				[eSolve] = {.order = 15, .argNo = 1, .fp = &round, .mat_op = 0, ._enum = eSolve},

				[eZeros] = {.order = 15, .argNo = 1, .fp = &zeros, .mat_op = 1, ._enum = eZeros},
				[eOnes] = {.order = 15, .argNo = 1, .fp = &ones, .mat_op = 1, ._enum = eOnes},
				[eEye] = {.order = 15, .argNo = 1, .fp = &eye, .mat_op = 1, ._enum = eEye},
				[eRand] = {.order = 15, .argNo = 1, .fp = &rand_mat, .mat_op = 1, ._enum = eRand},
				[eSize] = {.order = 15, .argNo = 1, .fp = &get_size, .mat_op = 1, ._enum = eSize},
				[eNumel] = {.order = 15, .argNo = 1, .fp = &numel, .mat_op = 1, ._enum = eNumel},
				[eMagnitude] = {.order = 15, .argNo = 1, .fp = &magnitude, .mat_op = 1, ._enum = eMagnitude},
						       
				[eTranspose] = {.order = 15, .argNo = 1, .fp = &t_mat, .mat_op = 1, ._enum = eTranspose},
				[eLinspace] = {.order = 15, .argNo = 1, .fp = &linspace, .mat_op = 0, ._enum = eLinspace},
				[eRun] = {.order = 15, .argNo = 1, .fp = &runFile, .mat_op = 0, ._enum = eRun},
				[ePrint] = {.order = 15, .argNo = 1, .fp = &printLine, .mat_op = 0, ._enum = ePrint},
				empty_op
};

op_struct
O_STRUCT[OPERATOR_COUNT + 1] = {
				[eAdd] = {.order = 6, .argNo = 2, .fp = &add, .mat_op = 0, ._enum = eAdd},
				[eSubtract] = {.order = 6, .argNo = 2, .fp = &subtract, .mat_op = 0, ._enum = eSubtract},
				[eMultiply] = {.order = 5, .argNo = 2, .fp = &multiply, .mat_op = 0, ._enum = eMultiply},
				[eDivide] = {.order = 5, .argNo = 2, .fp = &divide, .mat_op = 0, ._enum = eDivide},

				[eExponent] = {.order = 4, .argNo = 2, .fp = &exponent, .mat_op = 0, ._enum = eExponent},
				[eAssign] = {.order = 16, .argNo = 3, .fp = &assign, .mat_op = eAssign, ._enum = eAssign},
				[eLeftParen] = {.order = 16, .argNo = 0, .fp = NULL, .mat_op = eLeftParen, ._enum = eLeftParen},
				[eRightParen] = {.order = 15, .argNo = 0, .fp = NULL, .mat_op = eRightParen, ._enum = eRightParen},

				[eMultiplyMatrix] = {.order = 5, .argNo = 2, .fp = &mult_mat, .mat_op = 1, ._enum = eMultiplyMatrix},
				[eDivideMatrix] = {.order = 5, .argNo = 2, .fp = &div_mat, .mat_op = 1, ._enum = eDivideMatrix},
				[eExponentMatrix] = {.order = 4, .argNo = 2, .fp = &exp_mat, .mat_op = 1, ._enum = eExponentMatrix},
				[eModulo] = {.order = 5, .argNo = 2, .fp = &modulo, .mat_op = 0, ._enum = eModulo},

				[eLess] = {.order = 8, .argNo = 2, .fp = &less, .mat_op = 0, ._enum = eLess},
				[eGreater] = {.order = 8, .argNo = 2, .fp = &greater, .mat_op = 0, ._enum = eGreater},
				[eLessEqual] = {.order = 8, .argNo = 2, .fp = &less_equal, .mat_op = 0, ._enum = eLessEqual},
				[eGreaterEqual] = {.order = 8, .argNo = 2, .fp = &greater_equal, .mat_op = 0, ._enum = eGreaterEqual},

				[eNotEqual] = {.order = 9, .argNo = 2, .fp = &not_equal, .mat_op = 0, ._enum = eNotEqual},
				[eEqual] = {.order = 9, .argNo = 2, .fp = &equal, .mat_op = 0, ._enum = eEqual},

				[eAnd] = {.order = 13, .argNo = 2, .fp = &and, .mat_op = 0, ._enum = eAnd},
				[eOr] = {.order = 14, .argNo = 2, .fp = &or, .mat_op = 0, ._enum = eOr},
				[eNot] = empty_op,

				[eReference] = {.order = 16, .argNo = 2, .fp = &reference, .mat_op = eReference},
				
};
