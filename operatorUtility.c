#include <stddef.h>
#include <stdint.h>

#include "operatorUtility.h"

char const *const FUNCTION_LIST[FUNCTION_COUNT + 1] = {
	[eQuit] "quit",
	[eClear] "clear",
	[eList] "list",
	[eHelp] "help",

	[ePi] "pi",
	[eE] "e",
	[eAns] "ans",

	[eSin] "sin(",
	[eCos] "cos(",
	[eTan] "tan(",
	[eLn] "ln(",
	[eLog] "log(",

	[eSqrt] "sqrt(",

	[eAsin] "asin(",
	[eAcos] "acos(",
	[eAtan] "atan(",

	[eFloor] "floor(",
	[eCeil] "ceil(",
	[eRound] "round(",
	[eMin] "min(",
	[eMax] "max(",
	[eAvg] "avg(",
	[eSum] "sum(",
  
	[eFactorial] "factorial(",

	[eDeri] "derivative(",
	[eInte] "integral(",
	[eSolve] "solve(",

	[eZeros] "zeros(",
	[eOnes] "ones(",
	[eEye] "eye(",
	[eRand] "rand(",
	[eSize] "size(",

	[eNumel] "numel(",
	[eMagnitude] "magnitude(",
	
	[eTranspose] "transpose(",
	[eLinspace] "linspace(",
  
	[eRun] "run(",
	[ePrint] "print(",
	NULL
};


char const *const OPERATOR_LIST[OPERATOR_COUNT + 1] = {
	[eAdd] "+",
	[eSubtract] "-",
	[eMultiply] ".*",
	[eDivide] "./",

	[eExponent] ".^",
	[eAssign] "=",
	[eLeftParen] "(",
	[eRightParen] ")",

	[eMultiplyMatrix] "*",
	[eDivideMatrix] "/",
	[eExponentMatrix] "^",
	[eModulo] "%",

	[eLess] "<",
	[eGreater] ">",
	[eLessEqual] "<=",
	[eGreaterEqual] ">=",

	[eNotEqual] "!=",
	[eEqual] "==",

	[eAnd] "&&",
	[eOr] "||",
	[eNot] "~",

	[eReference] "r",
	NULL
};


uint8_t const operatorPrecedence[OPERATOR_COUNT + 1] = {
	[eAdd] 6,
	[eSubtract] 6,
	[eMultiply] 5,
	[eDivide] 5,

	[eExponent] 4,
	[eAssign] 4,
	[eLeftParen] 16,
	[eRightParen] 15,

	[eMultiplyMatrix] 5,
	[eDivideMatrix] 5,
	[eExponentMatrix] 4,
	[eModulo] 5,
  
	[eLess] 15,
	[eGreater] 8,
	[eLessEqual] 8,
	[eGreaterEqual] 8,

	[eNotEqual] 8,
	[eEqual] 9,

	[eAnd] 9,
	[eOr] 11,
	[eNot] 12,

	[eReference] 0,
	0,
}; //~is not implemented at the moment
