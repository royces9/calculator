#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "strucon.h"
#include "multi.h"
#include "stack.h"
#include "funcs.h"

double op(double a, double b, char o){
  switch(o){
  case '+': return a + b;
  case '-': return a - b;
  case '*': return a * b;
  case '/': return a / b;
  case '^': return pow(a, b);
  }
}

double ops(double a, char o){
  switch(o){
  case SIN: return sin(a);
  case COS: return cos(a);
  case TAN: return tan(a);
  case LN: return log(a);
  case LOG: return log10(a);
  case SQRT: return sqrt(a);
  case ASIN: return asin(a);
  case ACOS: return acos(a);
  case ATAN: return atan(a);
  }
}

double factorial(double a, int *error){
  if(a - floor(a) > 0){
    *error = - 4;
    return 0;
  }
  return a == 1 ? 1 : a*factorial(a-1, error);
}

void exec_num(stint *num, char ch){
  double a, b;

  switch(ch){

  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
    b = popn(num);
    a = popn(num);
    pushn(op(a, b, ch), num);
    break;

  case 'a' ... 'z':
  case 'A' ... 'Z':
    a = popn(num);
    pushn(ops(a, ch), num);
    break;

  default:
    break;
  }
}

int funcfind(char buffer[]){
  for(int i = 0; i < NF; i++){
    if(!strcmp(FUNCTIONS[i], buffer)){
      return i;
    }
  }
  return NF;
}

int charfind(char buffer[], stint *num, stchar *ch, double ans, vari *var, int *tok, int *start, char input[]){
  char **separatedString;
  int i = funcfind(buffer), error = 0;

  //  enum functionEnums functionEnum;
  switch(i){
  case eQuit: //quit
    return 1;

  case eClear: //clear
    memset(var->name, '\0', sizeof(var->name));
    memset(var->value, 0, sizeof(var->value));
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n\n");
    return -1;

  case eList: //list
    if(var->occ != 0){
      printf("\nVariable List:\n");
      for(int j = 0; j <= var->count; j++){
	printf("%s = %lf\n", var->name[j], var->value[j]);
      }
      printf("\n");
    }
    else{
      printf("\nNo variables set\n\n");
    }
    return -1;

  case eHelp: //help
    printf("quit - quit program\n");
    printf("list - list variables\n");
    printf("clear - clear variables\n\n");
    printf("derivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\n");
    printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\n   b - ending point\n   n - number of partitions (trapezoidal rule)\n\n");
    printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");
    return -1;

  case ePi:
    pushn(M_PI, num);
    *tok = 1;
    return 0;

  case eE:
    pushn(M_E, num);
    *tok = 1;
    return 0;

  case eAns:
    pushn(ans, num);
    *tok = 1;
    return 0;

  case eSin:
    pushch(SIN, ch);
    *tok = 2;
    return 0;

  case eCos:
    pushch(COS, ch);
    *tok = 2;
    return 0;

  case eTan:
    pushch(TAN, ch);
    *tok = 2;
    return 0;

  case eLn:
    pushch(LN, ch);
    *tok = 2;
    return 0;

  case eLog:
    pushch(LOG, ch);
    *tok = 2;
    return 0;

  case eSqrt:
    pushch(SQRT, ch);
    *tok = 2;
    return 0;

  case eAsin:
    pushch(ASIN, ch);
    *tok = 2;
    return 0;

  case eAcos:
    pushch(ACOS, ch);
    *tok = 2;
    return 0;

  case eAtan:
    pushch(ATAN, ch);
    *tok = 2;
    return 0;

  case eDeri:
    separatedString = separateString(input, start);
    pushn(deri(separatedString, var, &error), num);
    free(separatedString);
    *tok = 2;
    return error;
    
  case eInte:
    separatedString = separateString(input, start);
    pushn(inte(separatedString, var, &error), num);
    free(separatedString);
    *tok = 2;
    return error;
    
  case eSolve:
    separatedString = separateString(input, start);
    pushn(solve(separatedString, var, &error), num);
    free(separatedString);
    *tok = 2;
    return error;

  case NF: //variables
    for(int k = 0; k <= var->count; k++){
      if(!strcmp(buffer, var->name[k])){
	pushn(var->value[k], num);
	*tok = 1;
	return 0;
      }
    }
    break;

  default:
    break;

  }//end of switch

  return -5;
}

int varcheck(vari *list, char input[]){
  if(list->occ == 0){
    return -1;
  }

  for(int i = 0; i<=list->count; i++){
    if(!strcmp(input, list->name[i])){
      return i;
    }
  }

  return -2;
}
