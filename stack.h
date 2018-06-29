#ifndef STACK
#define STACK

#include <stdint.h>

#include "matrix.h"

//macro for malloc failure
#define __MALLOC_CHECK(pointer, error) \
  if(pointer == NULL) {\
  error = -6; \
  return 0; \
  }


typedef struct { //struct for operators, +, -, etc
  //operator/function string
  char operator[16];

  //order of operation, higher is higher priority
  uint8_t precedence;

  //number of arguments it takes, generally two or one
  uint8_t argNo;

  //corresponding enum, in operatorUtility.h
  uint8_t enumeration;

} operatorStruct;


typedef struct { //struct for stack of numbers
  //stack array
  matrix *stk[1024];

  //index for the top index
  //-1 is empty
  int top;

} numberStack;


typedef struct { //same as numberStack, except for operators
  //stack array
  operatorStruct stk[1024];

  //index for the top index
  //-1 is empty
  int top;

} operatorStack;


typedef struct { //variable storage
  //pointer to matrix index to assign to, if assignment
  matrix *assignIndex;

  //stores variable values
  matrix *value[256];

  //stores variable names
  char *name[256];

  //answer matrix
  matrix *ans;

  //index for the newest variable
  //-1 if empty
  int count;

  //flag if an assignment occured
  int8_t assignFlag;
} vari;


void pushn(matrix *inp, numberStack *st);
matrix *popn(numberStack *st);

void pushch(operatorStruct inp, operatorStack *st);
operatorStruct popch(operatorStack *st);

operatorStruct initOperatorStruct(const char *operator, uint8_t argNo, uint8_t precedence, uint8_t enumeration);

numberStack *newNumberStack(void);
operatorStack *newOperatorStack(void);

vari *newVari(void);
vari *copyVari(vari *var, error_return *error);

error_return setVariable(vari *var, char *name, matrix *a, int *check);
void freeVari(vari *var);

void freeNumberStack(numberStack *st);
void freeOperatorStack(operatorStack *st);

#endif //STACK
