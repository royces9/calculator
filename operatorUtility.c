#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include "types.h"
#include "functions.h"
#include "operator.h"
#include "multi.h"
#include "operatorUtility.h"

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
					      &sum

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
					      &llinspace,

					      &run,
					      &printLine
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

op_struct const *const F_STRUCT[FUNCTION_COUNT + 1] = {
						       [eQuit] = NULL,
						       [eClear] = NULL,
						       [eList] = NULL,
						       [eHelp] = NULL,

						       [ePi] = NULL,
						       [eE] = NULL,
						       [eAns] = NULL,

						       [eSin] = &{order = 15, argNo = 1, _enum = eSin, fp = &sin, mat_op = 0},
						       [eCos] = &{order = 15, argNo = 1, _enum = eCos, fp = &cos, mat_op = 0},
						       [eTan] = &{order = 15, argNo = 1, _enum = eTan, fp = &tan, mat_op = 0},
						       [eLn] = &{order = 15, argNo = 1, _enum = eLn, fp = &log, mat_op = 0},
						       [eLog] = &{order = 15, argNo = 1, _enum = eLog, fp = &log10, mat_op = 0},

						       [eSqrt] = &{order = 15, argNo = 1, _enum = eSqrt, fp = &sqrt, mat_op = 0},

						       [eAsin] = &{order = 15, argNo = 1, _enum = eAsin, fp = &asin, mat_op = 0},
						       [eAcos] = &{order = 15, argNo = 1, _enum = eAcos, fp = &acos, mat_op = 0},
						       [eAtan] = &{order = 15, argNo = 1, _enum = eAtan, fp = &atan, mat_op = 0},

						       [eFloor] = &{order = 15, argNo = 1, _enum = eFloor, fp = &floor, mat_op = 0},
						       [eCeil] = &{order = 15, argNo = 1, _enum = eCeil, fp = &ceil, mat_op = 0},
						       [eRound] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						       [eMin] = &{order = 15, argNo = 1, _enum = eMin, fp = &min, mat_op = 1},
						       [eMax] = &{order = 15, argNo = 1, _enum = eMax, fp = &max, mat_op = 1},
						       [eAvg] = &{order = 15, argNo = 1, _enum = eAvg, fp = &avg, mat_op = 1},
						       [eSum] = &{order = 15, argNo = 1, _enum = eSum, fp = &sum, mat_op = 1},

						       [eFactorial] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},

						       [eDeri] = &{order = 15, argNo = 1, _enum = eDeri, fp = &deri, mat_op = 0},
						       [eInte] = &{order = 15, argNo = 1, _enum = eInte, fp = &round, mat_op = 0},
						       [eSolve] = &{order = 15, argNo = 1, _enum = eSolve, fp = &round, mat_op = 0},

						[eZeros] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[eOnes] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[eEye] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[eRand] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[eSize] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},

						[eNumel] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[eMagnitude] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},

						[eTranspose] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[eLinspace] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},

						[eRun] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						[ePrint] = &{order = 15, argNo = 1, _enum = eRound, fp = &round, mat_op = 0},
						NULL
};

op_struct const O_STRUCT[OPERATOR_COUNT + 1] = {

};
