#ifndef UTILITY
#define UTILITY

//file including all the utility
//enums/constants/etc

#define FUNCTION_COUNT 38
#define OPERATOR_COUNT 22

extern char const *const FUNCTION_LIST[FUNCTION_COUNT + 1];
extern char const *const OPERATOR_LIST[OPERATOR_COUNT + 1];

extern struct oper O_STRUCT[OPERATOR_COUNT + 1];
extern struct oper F_STRUCT[FUNCTION_COUNT + 1];

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
