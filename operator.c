#include <math.h>
#include <string.h>
#include <stdio.h>

#include "operator.h"

//string containing functions
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

//string containing operators
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

//array containing the precedence of each operator
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

//search in FUNCTION_LIST
int searchFunctionArray(char *buffer) {
  for(int i = 0; i < FUNCTION_COUNT; ++i) {
    if(!strcmp(FUNCTION_LIST[i], buffer)) {
      return i;
    }
  }
  return FUNCTION_COUNT;
}

//search in OPERATOR_COUNT
int searchOperatorArray(char *buffer) {
  for(int i = 0; i < OPERATOR_COUNT; ++i) {
    if(!strcmp(OPERATOR_LIST[i], buffer)) {
      return i;
    }
  }
  return OPERATOR_COUNT;
}


//set the operatorStruct, kinda like a constructor
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration) {
  operatorStruct out;
  strcpy(out.operator, operator);
  out.precedence = precedence;
  out.argNo = argNo;
  out.enumeration = enumeration;
  return out;
}

//executes either one argument function or two argument function
void execNum(numberStack *num, operatorStruct ch) {
  matrix *a, *b;
  //temp error
  int *error = 0;
  switch(ch.argNo) {
  case 1:
    a = popn(num);
    pushn(matrixOneArg(a, ch.enumeration), num);
    break;

  case 2:
    b = popn(num);
    a = popn(num);
    pushn(matrixTwoArg(a, b, ch.enumeration, error), num);
    break;

  default:
    break;
  }
}


//factorial function
element factorial(element a) {
  a = floor(a);
  if(a == 0) {
    return 1;
  }
  return a == 1 ? 1 : a*factorial(a-1);
}


//returns value from one argument functions
element oneArg(element a, int o) {
  switch(o) {
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


//returns value from two argument function
element twoArg(element a, element b, int o) {
  switch(o) {
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


int findOperator(char *buffer, numberStack *num, operatorStack *oper, matrix ans, vari *var, int *tok) {
  int i = searchOperatorArray(buffer);
  int error = 0;

  /*                                                                                                                  
  **Precedence values for operators: Reference wiki page of C/C++ operators
  **1                                                
  **2 f(x)-calls
  **3
  **4 ^ !
  **5
  **6 + -
  **7
  **8 < <= > >=
  **9 == !=
  **10
  **11 &&
  **12 ||
  **13
  **14
  **15 parens
  **16 =
*/
  switch(i) {
  case eSubtract:
    if(*tok == 1) {
      while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1) {
	execNum(num, popch(oper));
      }
      pushch(setOpStack("+", 2, 6, eAdd), oper);
    }

    *tok = 0;
    pushch(setOpStack("*", 2, 5, eMultiply), oper);
    pushn(initScalar(-1), num);
    break;

  case eExponent:
    *tok = 0;
    pushch(setOpStack("^", 2, 4, eExponent), oper);
    break;

  case eLeftParen:
    *tok = 0;
    pushch(setOpStack("(", 1, 15, eLeftParen), oper);
    break;

  case eRightParen:
    do {
      execNum(num, popch(oper));
    } while(strcmp(oper->stk[oper->top].operator, "(") && oper->occ == 1);

    *tok = 1;
    popch(oper);
    break;


  case eAssign:
    break;

  case eAdd:
  case eMultiply:
  case eDivide:
  case eLess:
  case eGreater:
  case eLessEqual:
  case eGreaterEqual:
  case eNotEqual:
  case eEqual:
  case eAnd:
  case eOr:
    while((oper->stk[oper->top].precedence <= operatorPrecedence[i]) && (oper->occ == 1)) {
      execNum(num, popch(oper));
    }
    *tok = 0;
    pushch(setOpStack(buffer, 2, operatorPrecedence[i], i), oper);
    break;

  default:
    return -7;
  }


  return 0;
}
