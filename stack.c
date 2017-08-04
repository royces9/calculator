//#include "structs.h"
#include "stack.h"

void pushn(double inp, numberStack *st){
  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }
  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

double popn(numberStack *st){
  double out;
  if(st->occ == 1){
    out = st->stk[st->top--];

    if(st->top == -1){
      st->occ = 0;
      st->top = 0;
    }
    return out;
  }
  else{
    return 0;
  }
}
//characters
void pushch(operatorStruct inp, operatorStack *st){
  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }

  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

operatorStruct popch(operatorStack *st){
  operatorStruct out;
  if(st->occ == 1){
    out = st->stk[st->top--];

    if(st->top == -1){
      st->stk[0].operator = '\0';
      st->stk[0].argNo = 0;
      st->occ = 0;
      st->top = 0;
    }

    return out;
  }
  else{
    out.operator = '\0';
    out.argNo = 0;
    return out;
  }
}
