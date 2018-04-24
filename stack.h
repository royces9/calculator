#ifndef STACK
#define STACK

#include "matrix.h"

//macro for malloc failure
#define __MALLOC_CHECK(pointer, error) \
  if(pointer == NULL) {\
  error = -6; \
  return 0; \
  }

typedef struct { //struct for operators, +, -, etc
  char operator[2]; //char holding operator, operators are only two chars wide for now
  int precedence; //order of operation, higher is higher priority
  int argNo; //number of arguments it takes, generally two or one
  int enumeration; //corresponding enum
} operatorStruct;


typedef struct { //struct for stack of numbers
  matrix *stk[1024]; //stack
  int top; //index for where the top is
  char occ; //boolean, 0 if empty, 1 if occupied
} numberStack;


typedef struct { //same as numberStack, except for operators
  operatorStruct stk[1024];
  int top;
  char occ;
} operatorStack;


typedef struct { //variable storage
  char name[256][256]; //stores variable names
  matrix *value[256]; //stores variable values
  char occ; //same as numberStack
  int count; //index for the newest variable
} vari;


void pushn(matrix *inp, numberStack *st);
matrix *popn(numberStack *st);
void pushch(operatorStruct inp, operatorStack *st);
operatorStruct popch(operatorStack *st);
numberStack newNumberStack(void);
operatorStack newOperatorStack(void);
vari newVari(void);

#endif //STACK
