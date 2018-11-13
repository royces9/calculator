#include <stddef.h>
#include <stdint.h>

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
