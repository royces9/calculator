#ifndef OPERATOR
#define OPERATOR

#include "stack.h"

#define __NF__ 28
#define __NO__ 18

extern const char __OPERATORS__[__NO__][5];
extern const char __FUNCTIONS__[__NF__][20];

enum functionEnums{
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

  eRun,
  ePrint
};

enum operatorEnum{
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

extern const int operatorPrecedence[__NO__];

int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
void execNum(numberStack *num, operatorStruct ch);
double oneArg(double a, int o);
double twoArg(double a, double b, int o);

#endif //OPERATOR
