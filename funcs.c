#include <math.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "funcs.h"
#include "multi.h"

#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442

#define E 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260595630738132328627943490763233829880753195251019011573

#define SIN 'a'
#define COS 'b'
#define TAN 'c'
#define LN 'd'
#define LOG 'e'
#define SQRT 'f'
#define ASIN 'g'
#define ACOS 'h'
#define ATAN 'i'

# define NF 18 //Number of functions+constants

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

  "integral(",
  "derivative"};


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


void exec_num(stint* num, char ch){

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

int charfind(char buffer[], stint* num, stchar* ch, double ans, vari* var, int* tok, int* start, char input){

  int i = funcfind(buffer);

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
    printf("quit - quit program\nlist - list variables\nclear - clear variables\n\n");
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

    pushn(multifunc(i,input,&start), num);
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

int varcheck(vari* list, char inp[]){

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
