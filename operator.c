#include <math.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "operator.h"

const char __FUNCTIONS__[NF][20] = {
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

  "derivative(",
  "integral(",
  "solve(",

  "run("
};

const char __OPERATORS__[NO][5] = {
  "+",
  "-",
  "*",
  "/",

  "^",
  "!",
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

int searchFunctionArray(char *buffer){
  for(int i = 0; i < NF; ++i){
    if(!strcmp(__FUNCTIONS__[i], buffer)){
      return i;
    }
  }
  return NF;
}

int searchOperatorArray(char *buffer){
  for(int i = 0; i < NO; ++i){
    if(!strcmp(__OPERATORS__[i], buffer)){
      return i;
    }
  }
  return NO;
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


