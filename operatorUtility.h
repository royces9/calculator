#ifndef UTILITY
#define UTILITY

//file including all the utility
//enums/constants/etc

#define FUNCTION_COUNT 31
#define OPERATOR_COUNT 18

/*
extern const char OPERATOR_LIST[OPERATOR_COUNT][5];
extern const char FUNCTION_LIST[FUNCTION_COUNT][20];
extern const int operatorPrecedence[OPERATOR_COUNT];
*/

const char FUNCTION_LIST[FUNCTION_COUNT][20] = {
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
  "factorial(",

  "derivative(",
  "integral(",
  "solve(",

  "zeros(",
  "ones(",
  "eye(",

  "run(",
  "print("
};


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

  eRun,
  ePrint
};


const char OPERATOR_LIST[OPERATOR_COUNT][5] = {
  "+",
  "-",
  "*",
  "/",

  "^",
  "=",
  "(",
  ")",

  "<",
  ">",
  "<=",
  ">=",

  "!=",
  "==",

  "&&",
  "||",
    "~"
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


const int operatorPrecedence[OPERATOR_COUNT] = {
  6,
  6,
  5,
  5,
  4,
  4,
  16,
  15,
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

#endif //UTILITY
