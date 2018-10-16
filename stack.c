#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"

void push(stack *stk, void *d) {
	stk->stk[++stk->top] = d;
}


void *pop(stack *stk) {
	void *out = NULL;

	if(stk->top > -1)
		out = stk->stk[stk->top--];

	return out;
}


//initialize operatorStruct
op_struct *init_op_struct(const char *op, uint8_t argNo, uint8_t order, uint8_t _enum){
	op_struct *out = malloc(sizeof(*out));
	
	strcpy(out->op, op);

	out->order = order;
	out->argNo = argNo;
	out->_enum = _enum;

	return out;
}


void *new_stk(int size) {
	stack *out = malloc(sizeof(*out));
	if(!out)
		return NULL;

	//allocate pointers
	out->stk = malloc(sizeof(int *) * size);
	if(!out->stk)
		return NULL;

	out->top = -1;

	return out;
}


void free_stk(stack *stk, void (*free_fun)(void *)) {
	if(free_fun)
		while(stk->top > -1)
			free_fun(stk->stk[stk->top--]);

	free(stk->stk);
	free(stk);
}
