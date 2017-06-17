#include <math.h>
#include <string.h>
#include <stdio.h>

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

int charfind(char buffer[], stint *num, stchar *ch, double ans, vari *var, int *tok, int *start, char inp[], int *error){

  int i = funcfind(buffer);
  //  printf("%s\n", buffer);
  switch(i){

  case 0: //quit
    return 101;

  case 1: //clear
    memset(var->name, '\0', sizeof(var->name));
    memset(var->value, 0, sizeof(var->value));
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n\n");
    return 1;

  case 2: //list
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
    return 1;

  case 3: //help
    printf("quit - quit program\nlist - list variables\nclear - clear variables\n\nderivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\nintegral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\n   b - ending point\n   n - number of partitions (trapezoidal rule)\n\n");
    return 1;

  case 4:
    pushn(PI, num);
    *tok = 1;
    return 0;

  case 5:
    pushn(E, num);
    *tok = 1;
    return 0;

  case 6:
    pushn(ans, num);
    *tok = 1;
    return 0;

  case 7:
    pushch(SIN, ch);
    *tok = 2;
    return 0;

  case 8:
    pushch(COS, ch);
    *tok = 2;
    return 0;

  case 9:
    pushch(TAN, ch);
    *tok = 2;
    return 0;

  case 10:
    pushch(LN, ch);
    *tok = 2;
    return 0;

  case 11:
    pushch(LOG, ch);
    *tok = 2;
    return 0;

  case 12:
    pushch(SQRT, ch);
    *tok = 2;
    return 0;

  case 13:
    pushch(ASIN, ch);
    *tok = 2;
    return 0;

  case 14:
    pushch(ACOS, ch);
    *tok = 2;
    return 0;

  case 15:
    pushch(ATAN, ch);
    *tok = 2;
    return 0;

  case 16:
  case 17:
    pushn(multifunc(i,inp,start, var, error), num);
    *tok = 2;
    return 0;

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

  return -1;
}

int varcheck(vari *list, char inp[]){

  if(list->occ == 0){
    return -1;
  }

  for(int i = 0; i<=list->count; i++){
    if(!strcmp(inp, list->name[i])){
      return i;
    }
  }

  return -2;
}
