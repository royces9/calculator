#ifndef UTILITY
#define UTILITY

//file including all the utility
//enums/constants/etc

#define FUNCTION_COUNT 34
#define OPERATOR_COUNT 21

extern const char OPERATOR_LIST[OPERATOR_COUNT][5];
extern const char FUNCTION_LIST[FUNCTION_COUNT][20];
extern const int operatorPrecedence[OPERATOR_COUNT];

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
  eFactorial,

  eDeri,
  eInte,
  eSolve,

  eZeros,
  eOnes,
  eEye,
  eSize,
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
  eMatrixExponent,

  eLess,
  eGreater,
  eLessEqual,
  eGreaterEqual,

  eNotEqual,
  eEqual,

  eAnd,
  eOr,
  eNot
};


#endif //UTILITY
