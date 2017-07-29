#ifndef STACK
#define STACK

void pushn(double inp, numberStack *st);
double popn(numberStack *st);
void pushch(operatorStruct inp, operatorStack *st);
operatorStruct popch(operatorStack *st);

#endif //STACK
