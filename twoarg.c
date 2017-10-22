#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "operator.h"

#include "twoarg.h"

int findOperator(char *buffer, numberStack *num, operatorStack *oper, double ans, vari *var, int *tok){
  int i = searchOperatorArray(buffer);
  int varset = 0, error = 0;

  /*
Precedence values for operators: Reference wiki page of C/C++ operators
1
2 f(x)-calls
3
4 ^ !
5 * /
6 + -
7
8 < <= > >=
9 == !=
10
11 &&
12 ||
13
14
15 parens
16 =
   */
  switch(i){
  case eSubtract:
    if(*tok == 2){
      pushn(-1, num);
      pushch(setOpStack("*", 2, 5, eMultiply), oper);
      *tok = 1;
      break;
    }
    while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("-", 2, 6, eSubtract), oper);
    break;

  case eAdd:
    while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("+", 2, 6, eAdd), oper);
    break;

  case eMultiply:
    while((oper->stk[oper->top].precedence <= 5) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }

    *tok = 2;
    pushch(setOpStack("*", 2, 5, eMultiply), oper);
    break;                         

  case eDivide:
    while((oper->stk[oper->top].precedence <= 5) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("/", 2, 5, eDivide), oper);
    break;                         


  case eExponent:
    *tok = 2;
    pushch(setOpStack("^", 2, 4, eExponent), oper);
    break;

  case eLeftParen:
    *tok = 2;
    pushch(setOpStack("(", 1, 15, eLeftParen), oper);
    break;
                           
  case eRightParen:
    while(strcmp(oper->stk[oper->top].operator, "(") && oper->occ == 1){
      execNum(num, popch(oper));
    }

    *tok = 1;
    popch(oper);
    break;

  case eAssign:
    break;

  case eFactorial:
    pushn(factorial(popn(num), &error), num);
    break;

  case eLess:
    while((oper->stk[oper->top].precedence <= 8) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("<", 2, 8, eLess), oper);
    break;
    
  case eGreater:
    while((oper->stk[oper->top].precedence <= 8) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack(">", 2, 8, eGreater), oper);
    break;
    
  case eLessEqual:
    while((oper->stk[oper->top].precedence <= 8) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("<=", 2, 8, eLessEqual), oper);
    break;
    
  case eGreaterEqual:
    while((oper->stk[oper->top].precedence <= 8) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack(">=", 2, 8, eGreaterEqual), oper);
    break;
    
  case eNotEqual:
    while((oper->stk[oper->top].precedence <= 8) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("!=", 2, 9, eNotEqual), oper);
    break;
    
  case eEqual:
    while((oper->stk[oper->top].precedence <= 8) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("==", 2, 8, eEqual), oper);
    break;

  case eAnd:
    while((oper->stk[oper->top].precedence <= 11) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("&&", 2, 11, eAnd), oper);
    break;

  case eOr:
    while((oper->stk[oper->top].precedence <= 12) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("||", 2, 12, eOr), oper);
    break;

    default:
      break;
  }

  return 0;
}

double factorial(double a, int *error){
  if(a - floor(a) > 0 || a <= 0){
    if(a == 0){
      return 1;
    }
    *error = - 4;
    return 0;
  }
  return a == 1 ? 1 : a*factorial(a-1, error);
}
