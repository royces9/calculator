#ifndef STACK
#define STACK

//macro for malloc failure
#define __MALLOC_CHECK(pointer, error) \
  if(pointer == NULL){\
  error = -6; \
  return 0; \
  }

typedef struct{ //struct for operators, +, -, etc
  char operator[10]; //char holding operator
  int precedence; //order of operation, higher is higher priority
  int argNo; //number of arguments it takes, generally two or one
  int enumeration; //corresponding enum
} operatorStruct;

typedef struct{ //struct for stack of numbers
  double stk[1024]; //stack
  int top; //index for where the top is
  char occ; //boolean, 0 if empty, 1 if occupied
} numberStack;

typedef struct{ //same as numberStack, except for operators
  operatorStruct stk[1024];
  int top;
  char occ;
} operatorStack;

typedef struct{ //variable storage
  char name[256][256]; //stores variable names
  double value[256]; //stores variable values
  char occ; //same as numberStack
  int count; //index for the newest variable
} vari;


void pushn(double inp, numberStack *st);
double popn(numberStack *st);
void pushch(operatorStruct inp, operatorStack *st);
operatorStruct popch(operatorStack *st);
numberStack newNumberStack();
operatorStack newOperatorStack();

#endif //STACK
