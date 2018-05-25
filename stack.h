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
  int precedence; //order of operation, higher is higher priority
  int argNo; //number of arguments it takes, generally two or one
  int enumeration; //corresponding enum
  char operator[16]; //char holding operator/function
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
  matrix *assignIndex; //pointer to matrix index to assign to, if assignment
  matrix *value[256]; //stores variable values
  char *name[256]; //stores variable names
  matrix *ans; //answer matrix
  int count; //index for the newest variable
  int8_t assignFlag; //flag if an assignment occured
  char occ; //same as numberStack
} vari;


void pushn(matrix *inp, numberStack *st);
matrix *popn(numberStack *st);

void pushch(operatorStruct inp, operatorStack *st);
operatorStruct popch(operatorStack *st);

operatorStruct initOperatorStruct(const char *operator, int argNo, int precedence, int enumeration);

numberStack *newNumberStack(void);
operatorStack newOperatorStack(void);

vari *newVari(void);
vari *copyVari(vari *var, error_return *error);

error_return setVariable(vari *var, char *name, char check);
void freeVari(vari *var);

void freeNumberStack(numberStack *st);

#endif //STACK
