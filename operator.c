#include <math.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "operator.h"

const char __FUNCTIONS__[__NF__][20] = {
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

  "run(",
  "print("
};

const char __OPERATORS__[__NO__][5] = {
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

const int operatorPrecedence[__NO__] = {
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

int searchFunctionArray(char *buffer){
  for(int i = 0; i < __NF__; ++i){
    if(!strcmp(__FUNCTIONS__[i], buffer)){
      return i;
    }
  }
  return __NF__;
}

int searchOperatorArray(char *buffer){
  for(int i = 0; i < __NO__; ++i){
    if(!strcmp(__OPERATORS__[i], buffer)){
      return i;
    }
  }
  return __NO__;
}

operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration){
  operatorStruct out;
  strcpy(out.operator, operator);
  out.precedence = precedence;
  out.argNo = argNo;
  out.enumeration = enumeration;
  return out;
}

void execNum(numberStack *num, operatorStruct ch){
  double a, b;
  switch(ch.argNo){
  case 1:
    a = popn(num);
    pushn(oneArg(a, ch.enumeration), num);
    break;

  case 2:
    b = popn(num);
    a = popn(num);
    pushn(twoArg(a, b, ch.enumeration), num);
    break;

  default:
    break;
  }
}

double factorial(double a){
  a = floor(a);
  if(a == 0){
    return 1;
  }
  return a == 1 ? 1 : a*factorial(a-1);
}

double oneArg(double a, int o){
  switch(o){
  case eSin: return sin(a);
  case eCos: return cos(a);
  case eTan: return tan(a);
  case eLn: return log(a);
  case eLog: return log10(a);
  case eSqrt: return sqrt(a);
  case eAsin: return asin(a);
  case eAcos: return acos(a);
  case eAtan: return atan(a);
  case eFloor: return floor(a);
  case eCeil: return ceil(a);
  case eRound: return round(a);
  case eFactorial: return factorial(a);
  default: return a;
  }
}

double twoArg(double a, double b, int o){
  switch(o){
  case eAdd: return a + b;
  case eSubtract: return a - b;
  case eMultiply: return a * b;
  case eDivide: return a / b;
  case eExponent: return pow(a, b);
  case eLess: return a < b;
  case eGreater: return a > b;
  case eLessEqual: return a <= b;
  case eGreaterEqual: return a >= b;
  case eNotEqual: return a != b;
  case eEqual: return a == b;
  case eAnd: return a && b;
  case eOr: return a || b;
  }
}
