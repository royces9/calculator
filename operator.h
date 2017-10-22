#ifndef OPERATOR
#define OPERATOR

#include "stack.h"

#define NF 26
#define NO 15

extern const char __OPERATORS__[NO][5];
extern const char __FUNCTIONS__[NF][20];

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

  eDeri,
  eInte,
  eSolve,

  eRun
};

enum operatorEnum{
  eAdd,
  eSubtract,
  eMultiply,
  eDivide,

  eExponent,
  eFactorial,
  eAssign,
  eLeftParen,
  eRightParen,

  eLess,
  eGreater,
  eLessEqual,
  eGreaterEqual,

  eNotEqual,
  eEqual
};

int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence);
void execNum(numberStack *num, operatorStruct ch);
double oneArg(double a, char *o);
double twoArg(double a, double b, char *o);

#endif //OPERATOR
