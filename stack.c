#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

void pushn(matrix *inp, numberStack *st) {
  st->stk[++st->top] = inp;
}


matrix *popn(numberStack *st) { //pop a matrix from the stack
  matrix *out = NULL;

  if(st->top > -1){
    out = st->stk[st->top--];
  }

  return out;
}


//characters
void pushch(operatorStruct inp, operatorStack *st) {
  st->stk[++st->top] = inp;
}


operatorStruct popch(operatorStack *st) {
  operatorStruct out;

  if(st->top > -1){
    out = st->stk[st->top--];

  } else {
    out.operator[0] = '\0';
    out.argNo = 0;
  }

  return out;
}


//initialize operatorStruct
operatorStruct initOperatorStruct(const char *operator, uint8_t argNo, uint8_t precedence, uint8_t enumeration){
  operatorStruct out;

  strcpy(out.operator, operator);

  out.precedence = precedence;
  out.argNo = argNo;
  out.enumeration = enumeration;

  return out;
}


numberStack *newNumberStack(void) { //make new number stack
  numberStack *out = calloc(1, sizeof(*out));
  out->top = -1;
  return out;
}


operatorStack *newOperatorStack(void) { //make new operator stack
  operatorStack *out = calloc(1, sizeof(*out));
  out->top = -1;

  return out;
}


vari *newVari(void) {
  vari *var = calloc(1, sizeof(*var));
  var->ans = calloc(1, sizeof(*var->ans));
  var->count = -1;
  return var;
}


vari *copyVari(vari *var, error_return *error){
  vari *out = malloc(sizeof(*out));

  out->ans = calloc(1, sizeof(*out->ans));
  
  out->assignIndex = NULL;
  out->assignFlag = var->assignFlag;
  
  out->count = var->count;


  if(var->count > -1){
    int i = 0;
    for(; i < var->count; ++i){
	out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
	strcpy(out->name[i], var->name[i]);

	out->value[i] = copyMatrix(var->value[i], error);
	out->value[i]->variable = 1;
    }


    if((var->value[i] != NULL) && (var->value[i]->size != NULL)){
      out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
      strcpy(out->name[i], var->name[i]);

      out->value[i] = copyMatrix(var->value[i], error);
      out->value[i]->variable = 1;
    }
    ++i;
    out->name[i] = NULL;
    out->value[i] = NULL;
  } else{
    memset(out->name, '\0', sizeof(out->name));
    memset(out->value, 0, sizeof(out->value));
  }

  return out;
}


error_return setVariable(vari *var, char *name, matrix *a, int *check){
  //check is from the output of varcheck

  int index = 0;
  error_return error = 0;

  switch(*check){
  case -1: //new variable, struct is empty
    index = 0;
    var->count = 0;
    break;
    
  case -2: //new variable, struct is not empty
    index = ++var->count;
    break;

  default: //variable exists already
    index = *check;
    free(var->name[index]);
    var->value[index]->variable = 0;
    freeMatrix(var->value[index]);
    break;
  }

  *check = index;

  var->name[index] = malloc(sizeof(*var->name[index]) * (strlen(name) + 1));
  __MALLOC_CHECK(var->name[index], error);
  strcpy(var->name[index], name);
  var->value[index] = a;

  if((*check == -1) || (*check == -2)){
    var->name[index + 1] = NULL;
    var->value[index + 1] = NULL;
  }

  return error;
}


void freeVari(vari *var){

  for(int i = 0; var->name[i]; ++i){
    free(var->name[i]);
  }

  for(int i = 0; var->value[i]; ++i){
    var->value[i]->variable = 0;
    freeMatrix(var->value[i]);
  }


  if(var->ans->size != NULL){
    free(var->ans->size);
    free(var->ans->elements);
  }

  free(var->ans);

  if(var->assignIndex != NULL){
    freeMatrix(var->assignIndex);
  }

  free(var);
}


void freeNumberStack(numberStack *st){
  while(st->top > -1){
    if(st->stk[st->top] != NULL){
      freeMatrix(popn(st));
    } else{
      st->top--;
    }
  }

  free(st);
}

void freeOperatorStack(operatorStack *st){
  free(st);
}
