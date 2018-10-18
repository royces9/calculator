#ifndef UTILITY
#define UTILITY

//file including all the utility
//enums/constants/etc

#define FUNCTION_COUNT 38
#define OPERATOR_COUNT 23

extern char const *const FUNCTION_LIST[FUNCTION_COUNT];
extern char const *const OPERATOR_LIST[OPERATOR_COUNT];

extern uint8_t const operatorPrecedence[OPERATOR_COUNT];


extern matrix * (*func_arr) (char **, vari *, int8_t *)[7];

enum functionEnums {
	eQuit,
	eClear,
	eList,
	eHelp,

	ePi,
	eE,
	eAns,

	eSin,
	eCos,
	eTan,
	eLn,
	eLog,

	eSqrt,

	eAsin,
	eAcos,
	eAtan,

	eFloor,
	eCeil,
	eRound,
	eMin,
	eMax,
	eAvg,
	eSum,

	eFactorial,

	eDeri,
	eInte,
	eSolve,

	eZeros,
	eOnes,
	eEye,
	eRand,
	eSize,
	eNumel,
	eMagnitude,
	
	eTranspose,
	eLinspace,

	eRun,
	ePrint
};


enum operatorEnum {
	eAdd,
	eSubtract,
	eMultiply,
	eDivide,

	eExponent,
	eAssign,
	eLeftParen,
	eRightParen,

	eMultiplyMatrix,
	eDivideMatrix,
	eExponentMatrix,
	eModulo,

	eLess,
	eGreater,
	eLessEqual,
	eGreaterEqual,

	eNotEqual,
	eEqual,

	eAnd,
	eOr,
	eNot,

	eReference
};


#endif //UTILITY
