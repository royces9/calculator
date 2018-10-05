#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"

void pushn(matrix *inp, numberStack *st) {
	st->stk[++st->top] = inp;
}


matrix *popn(numberStack *st) { //pop a matrix from the stack
	matrix *out = NULL;

	if(st->top > -1)
		out = st->stk[st->top--];

	return out;
}


//characters
void pushch(operatorStruct *inp, operatorStack *st) {
	st->stk[++st->top] = inp;
}


operatorStruct *popch(operatorStack *st) {
	operatorStruct *out = NULL;

	if(st->top > -1)
		out = st->stk[st->top--];

	return out;
}


//initialize operatorStruct
operatorStruct *initOperatorStruct(const char *operator, uint8_t argNo, uint8_t precedence, uint8_t enumeration){
	operatorStruct *out = malloc(sizeof(*out));

	strcpy(out->operator, operator);

	out->precedence = precedence;
	out->argNo = argNo;
	out->enumeration = enumeration;

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


void freeNumberStack(numberStack *st) {
	while(st->top > -1) {
		free_mat(st->stk[st->top--]);
	}
	free(st);
}

void freeOperatorStack(operatorStack *st) {
	while(st->top > -1) {
		free(st->stk[st->top--]);
	}
	free(st);
}
