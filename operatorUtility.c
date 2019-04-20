#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include "types.h"
#include "functions.h"
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
					      &print
					      NULL
};

void *const OPERATOR_POINTER[OPERATOR_COUNT + 1] = {

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

