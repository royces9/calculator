#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"


void pushn(matrix *inp, numberStack *st) {
  if(st->occ) {
    st->stk[++st->top] = inp;
  } else { //if stack is empty put it into 0, st.top is guaranteed to be 0 at initialization
    st->stk[0] = inp;
    st->occ = 1;
  }
}


matrix *popn(numberStack *st) { //pop a matrix from the stack
  matrix *out = NULL;
  if(st->occ) {
    out = st->stk[st->top--];
    if((st->top) == -1) { //if top is -1, the stack is now empty, set occ and top back to 0
      st->occ = 0;
      st->top = 0;
    }
  }
  return out;
}


//characters
void pushch(operatorStruct inp, operatorStack *st) {
  if(st->occ) {
    st->stk[++st->top] = inp;
  }

  else { //if stack is empty put it into 0
    st->stk[0] = inp;
    st->occ= 1;
  }
}


operatorStruct popch(operatorStack *st) {
  operatorStruct out;
  if(st->occ) {
    out = st->stk[st->top--];

    if(st->top == -1) { //if top is -1, then the stack is empty, set occ and top to 0
      st->occ = 0;
      st->top = 0;
    }
  } else {
    out.operator[0] = '\0';
    out.argNo = 0;
  }
  return out;
}


numberStack newNumberStack(void) { //make new number stack
  numberStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  return out;
}


operatorStack newOperatorStack(void) { //make new operator stack
  operatorStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, '\0', sizeof(out.stk));
  return out;
}


vari newVari(void) {
  vari var;
  var.count = 0;
  var.occ = 0;
  memset(var.name, '\0', sizeof(var.name));
  memset(var.value, 0, sizeof(var.value));

  var.ans.size = NULL;
  var.ans.elements = NULL;
  return var;
}


void freeVari(vari *var){
  if(var->occ != 0){
    for(int i = 0; i <= var->count; ++i){
      freeMatrix(var->value[i]);
    }
  }

  if(var->ans.size != NULL){
    free(var->ans.size);
    free(var->ans.elements);
  }
}


void emptyNumberStack(numberStack *st){
  while(st->occ == 1){
    free(popn(st));
  }
}
