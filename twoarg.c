#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "operator.h"
#include "onearg.h"
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
11
12parens
13
14
15
16 =
   */
  switch(i){
  case eSubtract:
    if(*tok == 2){
      pushn(-1, num);
      pushch(setOpStack("*", 2, 5), oper);
      *tok = 1;
      break;
    }
    while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("-", 2, 6), oper);
    break;

  case eAdd:
    while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("+", 2, 6), oper);
    break;

  case eMultiply:
    while((oper->stk[oper->top].precedence <= 5) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }

    *tok = 2;
    pushch(setOpStack("*", 2, 5), oper);
    break;                         

  case eDivide:
    while((oper->stk[oper->top].precedence <= 5) && (oper->occ == 1)){
      execNum(num, popch(oper));
    }
    *tok = 2;
    pushch(setOpStack("/", 2, 5), oper);
    break;                         


  case eExponent:
    *tok = 2;
    pushch(setOpStack("^", 2, 4), oper);
    break;

  case eLeftParen:
    *tok = 2;
    pushch(setOpStack("(", 1, 12), oper);
    break;
                           
  case eRightParen:
    do{
      //      printf("oper %s\n", oper->stk[oper->top].operator);
      //      printf("oper %lf %lf\n", num->stk[num->top], num->stk[num->top-1]);
      execNum(num, popch(oper));
    } while(strcmp(oper->stk[oper->top].operator, "(") && oper->occ == 1);
    *tok = 1;
    popch(oper);
    break;

  case eAssign:
    break;

  case eFactorial:
    pushn(factorial(popn(num), &error), num);
    break;

  case eLess:
  case eGreater:
  case eLessEqual:
  case eGreaterEqual:
  case eNotEqual:
  case eEqual:
    break;

    default:
      break;
  }

  return 0;
}
