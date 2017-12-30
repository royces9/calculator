#include <string.h>

#include "stack.h"

void pushn(double inp, numberStack *st){ //push a double onto stack
  if(st->occ == 1){ //if stack is occupied add onto the top
    st->stk[++st->top] = inp;
  }
  else{ //if stack is empty put it into 0, st.top is guaranteed to be 0 at initialization
    st->stk[0] = inp;
    st->occ= 1;
  }
}

double popn(numberStack *st){ //pop a double from the stack
  double out = 0;
  if(st->occ == 1){
    out = st->stk[st->top--]; //set out to the double at the top of the stack, then decrement top

    if((st->top) == -1){ //if top is -1, the stack is now empty, set occ and top back to 0
      st->occ = 0;
      st->top = 0;
    }
  }
  return out;
}
//characters
void pushch(operatorStruct inp, operatorStack *st){ //push an operator onto stack
  if(st->occ == 1){ //if stack is occupied add onto the top
    st->stk[++st->top] = inp;
  }

  else{ //if stack is empty put it into 0
    st->stk[0] = inp;
    st->occ= 1;
  }
}

operatorStruct popch(operatorStack *st){
  operatorStruct out;
  if(st->occ == 1){
    out = st->stk[st->top--]; //set out to what is at top of the stack, then decrement top

    if(st->top == -1){ //if top is -1, then the stack is empty, set occ and top to 0
      st->occ = 0;
      st->top = 0;
    }
  }
  else{
    out.operator[0] = '\0';
    out.argNo = 0;
  }
  return out;
}

numberStack newNumberStack(){ //make new number stack
  numberStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  return out;
}

operatorStack newOperatorStack(){ //make new operator stack
  operatorStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, '\0', sizeof(out.stk));
  return out;
}
