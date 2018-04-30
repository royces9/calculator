#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "operator.h"
#include "stack.h"
#include "multi.h"
#include "onearg.h"
#include "file.h"

int findFunction(char *buffer, numberStack *num, operatorStack *ch, matrix *ans, vari *var, int *tok, int *start, char *input) {
  char **separatedString;
  int i = searchFunctionArray(buffer), error = 0;
  operatorStruct operator;
  matrix *out;

  switch(i) {
  case eQuit: //quit
    return 1;

  case eClear: //clear
    freeVari(var);
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n\n");
    return -1;

  case eList: //list
    if(var->occ) {
      printf("\nVariable List:\n");
      for(int j = 0; j <= var->count; ++j) {
	printf("%s =", var->name[j]);
	printMatrix(*var->value[j]);
      }
      printf("\n");
    } else {
      printf("\nNo variables set\n\n");
    }
    return -1;

  case eHelp: //help
    helpPrint();
    return -1;

  case ePi:
    pushn(initScalar(M_PI), num);
    *tok = 1;
    return 0;

  case eE:
    pushn(initScalar(M_E), num);
    *tok = 1;
    return 0;

  case eAns:
    { //because ans scope is in main, make a copy of ans that can be free'd
      matrix *copy = malloc(sizeof(*copy));
      copyMatrix(copy, ans);
      pushn(copy, num);
      *tok = 1;
    }
    return 0;

  case eSin:
  case eCos:
  case eTan:
  case eLn:
  case eLog:
  case eSqrt:
  case eAsin:
  case eAcos:
  case eAtan:
  case eFloor:
  case eCeil:
  case eRound:
  case eFactorial:
  case eMin:
  case eMax:
  case eAvg:
    pushch(setOpStack(FUNCTION_LIST[i], 1, 2, i), ch);
    *tok = 0;
    return 0;
    /*
  case eMin:
    separatedString = separateString(input, ',', start, &error);
    pushn(min(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eMax:
    separatedString = separateString(input, ',', start, &error);
    pushn(max(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;    
    
  case eAvg:
    separatedString = separateString(input, ',', start, &error);
    pushn(avg(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;    
    */
  case eDeri:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(deri(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;
    
  case eInte:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(inte(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;
    
  case eSolve:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(solve(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eZeros:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(zeros(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;
    
  case eOnes:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(ones(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eRun:
    separatedString = separateString(input, "()", '\0', start, &error);
    error = runFile(separatedString, var, out);
    pushn(out, num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case ePrint:
    separatedString = separateString(input, "()", ',', start, &error);
    printLine(separatedString, var, &error);
    freeDoubleArray(separatedString);
    return error;

  case FUNCTION_COUNT: //variables
    {
      int k = varcheck(var, buffer);
      if(k >= 0) {
	matrix *varCopy = malloc(sizeof(*varCopy));
	pushn(copyMatrix(varCopy,var->value[k]), num);
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

matrix *extractMatrix(numberStack *num, operatorStack *ch, matrix *ans, vari *var, int *start, char *input, int *error){
  matrix *out;
  char **separatedString = separateString(input, "[]", ';', start, error);
  int rows = 0;
  int columns = 0;
  for(rows = 0; separatedString[rows]; ++rows){
    int *start2 = 0;
    char **separatedStringTwo = separateString(separatedString[rows], NULL, ',', start2, error);
    for(columns = 0; separatedStringTwo[columns]; ++columns){

    }
  }
  freeDoubleArray(separatedString);
  return out;
}

int varcheck(vari *list, char input[]) {
  if(list->occ == 0) {
    return -1;
  }

  for(int i = 0; i<=list->count; ++i) {
    if(!strcmp(input, list->name[i])) {
      return i;
    }
  }

  return -2;
}


void freeDoubleArray(char **input) {
  int i = 0;
  for(i = 0; strcmp(input[i], ""); ++i) {
    free(input[i]);
  }
  free(input[i]);
  free(input);
}


void helpPrint() {
    printf("quit - quit program\n");
    printf("list - list variables\n");
    printf("clear - clear variables\n\n");

    printf("derivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\n");

    printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\n   b - ending point\n   n - number of partitions (composite Simpson's rule, odd n is equivalent to n-1)\n\n");

    printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");

    printf("run(file)\n   file - path to a text file\n   This function parses each line of the file as if it were entered into the console directly, with the exception of \"while\", \"if/else\" and \"end\".\n    \"while\" - loops until the statement inside the \"while\"'s conditional is false. The inside is executed as if it were entered into the console directly. There may be floating point round off errors.\n   \"if/else\" - Executes the block of lines inside the \"if\"'s conditional if the statement is true, Otherwise it will execute the \"else\" block.\n   '#' at the beginning of a line comments out a line\n   ';' at the end of a line suppresses output\n\n");
}
