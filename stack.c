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

//initialize operatorStruct
operatorStruct initOperatorStruct(const char *operator, int argNo, int precedence, int enumeration){
  operatorStruct out;
  strcpy(out.operator, operator);
  out.precedence = precedence;
  out.argNo = argNo;
  out.enumeration = enumeration;
  return out;
}


numberStack *newNumberStack(void) { //make new number stack
  numberStack *out = malloc(sizeof(*out));
  out->top = 0;
  out->occ = 0;
  memset(out->stk, 0, sizeof(out->stk));
  return out;
}


operatorStack newOperatorStack(void) { //make new operator stack
  operatorStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, '\0', sizeof(out.stk));
  return out;
}


vari *newVari(void) {
  vari *var = malloc(sizeof(*var));
  var->count = 0;
  var->occ = 0;
  memset(var->name, '\0', sizeof(var->name));
  memset(var->value, 0, sizeof(var->value));

  var->ans = malloc(sizeof(*var->ans));
  var->ans->size = NULL;
  var->ans->elements = NULL;
  var->ans->variable = 0;

  var->assignIndex = NULL;

  return var;
}


vari *copyVari(vari *var, error_return *error){
  vari *out = malloc(sizeof(*out));

  out->ans = malloc(sizeof(*out->ans));
  out->ans->elements = NULL;
  out->ans->size = NULL;
  
  out->assignIndex = NULL;
  
  out->count = var->count;
  out->occ = var->occ;

  if(var->occ){
    int i = 0;
    for(; i < var->count; ++i){
	out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
	strcpy(out->name[i], var->name[i]);

	out->value[i] = copyMatrix(var->value[i], error);
    }

    if(var->value[i]->size != NULL){
      //if(!((var->name[i] != NULL) && (var->value[i]->size == NULL))){
      out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
      strcpy(out->name[i], var->name[i]);

      out->value[i] = copyMatrix(var->value[i], error);
      ++i;
    }

    out->name[i] = NULL;
    out->value[i] = NULL;

  } else{
    memset(out->name, '\0', sizeof(out->name));
    memset(out->value, 0, sizeof(out->value));
  }
  return out;
}


error_return setVariable(vari *var, char *name, char check){
  //check is from the output of varcheck

  int index = 0;
  error_return error = 0;

  switch(check){
  case -1: //new variable, struct is empty
    index = 0;
    var->occ = 1;
    break;
    
  case -2: //new variable, struct is not empty
    index = ++var->count;
    break;

  default: //variable exists already
    error = -5;
    break;
  }

  strcpy(var->name[index], name);

  return error;
}

void freeVari(vari *var){
  for(int i = 0; var->name[i] != NULL; ++i){
      var->value[i]->variable = 0;
      freeMatrix(var->value[i]);
      free(var->name[i]);
  }

  if(var->ans->size != NULL){
    free(var->ans->size);
  }

  if(var->ans->elements != NULL){
    free(var->ans->elements);
  }

  free(var->ans);

  if(var->assignIndex != NULL){
    freeMatrix(var->assignIndex);
  }

  free(var);
}


void freeNumberStack(numberStack *st){
  while(st->occ == 1){
    freeMatrix(popn(st));
  }

  free(st);
}
