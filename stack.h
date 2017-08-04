#ifndef STACK
#define STACK

typedef struct{
  char operator;
  int argNo;
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

#endif //STACK
