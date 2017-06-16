#include "strucon.h"
#include "stack.h"

void pushn(double inp, stint *st){

  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }
  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

double popn(stint *st){

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
void pushch(char inp, stchar *st){

  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }

  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

char popch(stchar *st){

  char out;
  if(st->occ == 1){
    out = st->stk[st->top--];

    if(st->top == -1){
      st->stk[0] == '\0';
      st->occ = 0;
      st->top = 0;
    }

    return out;
  }
  else{
    return 0;
  }
}
