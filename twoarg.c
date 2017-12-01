#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "operator.h"

#include "twoarg.h"

int findOperator(char *buffer, numberStack *num, operatorStack *oper, double ans, vari *var, int *tok){
  int i = searchOperatorArray(buffer);
  int error = 0;

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
    if(*tok == 1){
      while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1){
	execNum(num, popch(oper));
      }
      pushch(setOpStack("+", 2, 6, eAdd), oper);
    }
 
    *tok = 0;
    pushch(setOpStack("*", 2, 5, eMultiply), oper);
    pushn(-1, num);
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
    do{
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
    while((oper->stk[oper->top].precedence <= operatorPrecedence[i]) && (oper->occ == 1)){
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
