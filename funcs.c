#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "multi.h"
#include "funcs.h"
#include "file.h"

#define NF 26

const char FUNCTIONS[NF][20] = {
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

enum functionEnums{
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

  eDeri,
  eInte,
  eSolve,

  eRun
};

enum operations{
  opSin = 'a',
  opCos,
  opTan,
  opLn,
  opLog,
  opSqrt,
  opAsin,
  opAcos,
  opAtan,
  opFloor,
  opCeil,
  opRound,
  opMin,
  opMax,
  opAvg
};

operatorStruct setOpStack(char operator, int argNo){
  operatorStruct out;
  out.operator = operator;
  out.argNo = argNo;
  return out;
}

double twoArg(double a, double b, char o){
  switch(o){
  case '+': return a + b;
  case '-': return a - b;
  case '*': return a * b;
  case '/': return a / b;
  case '^': return pow(a, b);
  }
}

double oneArg(double a, char o){
  switch(o){
  case opSin: return sin(a);
  case opCos: return cos(a);
  case opTan: return tan(a);
  case opLn: return log(a);
  case opLog: return log10(a);
  case opSqrt: return sqrt(a);
  case opAsin: return asin(a);
  case opAcos: return acos(a);
  case opAtan: return atan(a);
  case opFloor: return floor(a);
  case opCeil: return ceil(a);
  case opRound: return round(a);    
  }
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

void exec_num(numberStack *num, operatorStruct ch){
  double a, b;
  switch(ch.argNo){
  case 1:
    a = popn(num);
    pushn(oneArg(a, ch.operator), num);
    break;

  case 2:
    b = popn(num);
    a = popn(num);
    pushn(twoArg(a, b, ch.operator), num);
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

int charfind(char buffer[], numberStack *num, operatorStack *ch, double ans, vari *var, int *tok, int *start, char input[]){
  char **separatedString;
  int i = funcfind(buffer), error = 0;
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
    pushch(setOpStack(opSin, 1), ch);
    *tok = 2;
    return 0;

  case eCos:
    pushch(setOpStack(opCos, 1), ch);
    *tok = 2;
    return 0;

  case eTan:
    pushch(setOpStack(opTan, 1), ch);
    *tok = 2;
    return 0;

  case eLn:
    pushch(setOpStack(opLn, 1), ch);
    *tok = 2;
    return 0;

  case eLog:
    pushch(setOpStack(opLog, 1), ch);
    *tok = 2;
    return 0;

  case eSqrt:
    pushch(setOpStack(opSqrt, 1), ch);
    *tok = 2;
    return 0;

  case eAsin:
    pushch(setOpStack(opAsin, 1), ch);
    *tok = 2;
    return 0;

  case eAcos:
    pushch(setOpStack(opAcos, 1), ch);
    *tok = 2;
    return 0;

  case eAtan:
    pushch(setOpStack(opAtan, 1), ch);
    *tok = 2;
    return 0;

  case eFloor:
    pushch(setOpStack(opFloor, 1), ch);
    *tok = 2;
    return 0;
    
  case eCeil:
    pushch(setOpStack(opCeil, 1), ch);
    *tok = 2;
    return 0;

  case eRound:
    pushch(setOpStack(opRound, 1), ch);
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

void freeDoubleArray(char **input){
  int i = 0;
  for(i = 0; strcmp(input[i], ""); i++){
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
