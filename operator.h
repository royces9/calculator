#ifndef OPERATOR
#define OPERATOR

#include "stack.h"

#define FUNCTION_COUNT 30
#define OPERATOR_COUNT 18

extern const char OPERATOR_LIST[OPERATOR_COUNT][5];
extern const char FUNCTION_LIST[FUNCTION_COUNT][20];

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

extern const int operatorPrecedence[OPERATOR_COUNT];

int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
void execNum(numberStack *num, operatorStruct ch);
double oneArg(double a, int o);
double twoArg(double a, double b, int o);
int findOperator(char *buffer, numberStack *num, operatorStack *ch, matrix ans, vari *var, int *tok);

#endif //OPERATOR
