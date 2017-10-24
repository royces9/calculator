#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "operator.h"
#include "stack.h"
#include "multi.h"
#include "onearg.h"
#include "file.h"

int findFunction(char *buffer, numberStack *num, operatorStack *ch, double ans, vari *var, int *tok, int *start, char input[]){
  char **separatedString;
  int i = searchFunctionArray(buffer), error = 0;
  operatorStruct operator;
  double out;

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
    printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\n   b - ending point\n   n - number of partitions (composite Simpson's rule, odd n is incremented to n+1)\n\n");
    printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");
    helpPrint();
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
    pushch(setOpStack(__FUNCTIONS__[eSin], 1, 2, eSin), ch);
    *tok = 2;
    return 0;

  case eCos:
    pushch(setOpStack(__FUNCTIONS__[eCos], 1, 2, eCos), ch);
    *tok = 2;
    return 0;

  case eTan:
    pushch(setOpStack(__FUNCTIONS__[eTan], 1, 2, eTan), ch);
    *tok = 2;
    return 0;

  case eLn:
    pushch(setOpStack(__FUNCTIONS__[eLn], 1, 2, eLn), ch);
    *tok = 2;
    return 0;

  case eLog:
    pushch(setOpStack(__FUNCTIONS__[eLog], 1, 2, eLog), ch);
    *tok = 2;
    return 0;

  case eSqrt:
    pushch(setOpStack(__FUNCTIONS__[eSqrt], 1, 2, eSqrt), ch);
    *tok = 2;
    return 0;

  case eAsin:
    pushch(setOpStack(__FUNCTIONS__[eAsin], 1, 2, eAsin), ch);
    *tok = 2;
    return 0;

  case eAcos:
    pushch(setOpStack(__FUNCTIONS__[eAcos], 1, 2, eAcos), ch);
    *tok = 2;
    return 0;

  case eAtan:
    pushch(setOpStack(__FUNCTIONS__[eAtan], 1, 2, eAtan), ch);
    *tok = 2;
    return 0;

  case eFloor:
    pushch(setOpStack(__FUNCTIONS__[eFloor], 1, 2, eFloor), ch);
    *tok = 2;
    return 0;
    
  case eCeil:
    pushch(setOpStack(__FUNCTIONS__[eCeil], 1, 2, eCeil), ch);
    *tok = 2;
    return 0;

  case eRound:
    pushch(setOpStack(__FUNCTIONS__[eRound], 1, 2, eRound), ch);
    *tok = 2;
    return 0;

  case eMin:
    separatedString = separateString(input, ',', start, &error);
    pushn(min(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;

  case eMax:
    separatedString = separateString(input, ',', start, &error);
    pushn(max(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;    
    
  case eAvg:
    separatedString = separateString(input, ',', start, &error);
    pushn(avg(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;    
    
  case eDeri:
    separatedString = separateString(input, ',', start, &error);
    pushn(deri(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;
    
  case eInte:
    separatedString = separateString(input, ',', start, &error);
    pushn(inte(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;
    
  case eSolve:
    separatedString = separateString(input, ',', start, &error);
    pushn(solve(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;

  case eRun:
    separatedString = separateString(input, '\0', start, &error);
    error = runFile(separatedString, var, &out);
    pushn(out, num);
    freeDoubleArray(separatedString);
    *tok = 2;
    return error;

  case NF: //variables
    {
      int k = varcheck(var, buffer);
      if(k >= 0){
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

  for(int i = 0; i<=list->count; ++i){
    if(!strcmp(input, list->name[i])){
      return i;
    }
  }

  return -2;
}

void freeDoubleArray(char **input){
  int i = 0;
  for(i = 0; strcmp(input[i], ""); ++i){
    free(input[i]);
  }
  free(input[i]);
  free(input);
}

void helpPrint(){
    printf("quit - quit program\n");
    printf("list - list variables\n");
    printf("clear - clear variables\n\n");

    printf("derivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\n");

    printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\n   b - ending point\n   n - number of partitions (composite Simpson's rule, odd n is incremented to n+1)\n\n");

    printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");

    printf("run(file)\n file - path to a text file\n This function parses each line of the file as if it were entered into the console directly.\n'#' at the beginning of a line comments out a line\n';' at the end of a line suppresses output\n\n");
}

