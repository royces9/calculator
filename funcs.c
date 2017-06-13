#include <math.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "funcs.h"

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

# define NF 16 //Number of functions+constants

const char FUNCTIONS[NF][10] = {
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
  
  "quit",
  "clear",
  "list",
  "help"};

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

//

int funcfind(char buffer[]){

  for(int i = 0; i < NF; i++){
    if(!strcmp(FUNCTIONS[i], buffer)){
      return i;
    }
  }
  return NF;
}


int charfind(char buffer[], stint* num, stchar* ch, double ans, vari* var, int* tok){

  int i = funcfind(buffer);

  switch(i){
  case 0:
    pushn(PI, num);
    *tok = 1;
    return 0;

  case 1:
    pushn(E, num);
    *tok = 1;
    return 0;

  case 2:
    pushn(ans, num);
    *tok = 1;
    return 0;

  case 3:
    pushch(SIN, ch);
    *tok = 2;
    return 0;

  case 4:
    pushch(COS, ch);
    *tok = 2;
    return 0;

  case 5:
    pushch(TAN, ch);
    *tok = 2;
    return 0;

  case 6:
    pushch(LN, ch);
    *tok = 2;
    return 0;

  case 7:
    pushch(LOG, ch);
    *tok = 2;
    return 0;

  case 8:
    pushch(SQRT, ch);
    *tok = 2;
    return 0;

  case 9:
    pushch(ASIN, ch);
    *tok = 2;
    return 0;

  case 10:
    pushch(ACOS, ch);
    *tok = 2;
    return 0;

  case 11:
    pushch(ATAN, ch);
    *tok = 2;
    return 0;

  case NF - 4: //quit
    return 2;

  case NF - 3: //clear
    memset(var->name, '\0', sizeof(var->name));
    memset(var->value, 0, sizeof(var->value));
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n\n");
    return 1;

  case NF - 2: //list
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

  case NF - 1: //help
    printf("quit - quit program\nlist - list variables\nclear - clear variable\
s\n\n");
    return 1;

  case NF: //variables
    for(i = 0; i <= var->count; i++){
      if(!strcmp(buffer, var->name[i])){
	pushn(var->value[i], num);
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

  int i = 0;

  if(list->occ == 0){
    return -1;
  }

  for(i = 0; i<=list->count; i++){
    if(!strcmp(inp, list->name[i])){
      return i;
    }
  }

  return -2;
}
