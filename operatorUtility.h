#ifndef UTILITY
#define UTILITY

//file including all the utility
//enums/constants/etc

#define FUNCTION_COUNT 38
#define OPERATOR_COUNT 23

//extern char const FUNCTION_LIST[FUNCTION_COUNT][16];
//extern char const OPERATOR_LIST[OPERATOR_COUNT][4];

extern char const *const FUNCTION_LIST[FUNCTION_COUNT];
extern char const *const OPERATOR_LIST[OPERATOR_COUNT];

extern uint8_t const operatorPrecedence[OPERATOR_COUNT];

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
