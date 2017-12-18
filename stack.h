#ifndef STACK
#define STACK

#define __MALLOC_CHECK(pointer, error) \
  if(pointer == NULL){\
  error = -6; \
  return 0; \
  }

typedef struct{
  char operator[10];
  int precedence;
  int argNo;
  int enumeration;
} operatorStruct;

typedef struct{
  double stk[1024];
  int top;
  int occ;
} numberStack;

typedef struct{
  operatorStruct stk[1024];
  int top;
  int occ;
} operatorStack;

typedef struct{
  char name[256][256];
  double value[256];
  int occ;
  int count;
} vari;


void pushn(double inp, numberStack *st);
double popn(numberStack *st);
void pushch(operatorStruct inp, operatorStack *st);
operatorStruct popch(operatorStack *st);
numberStack newNumberStack();
operatorStack newOperatorStack();

#endif //STACK
