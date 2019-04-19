#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

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
op_struct *init_op_struct(uint8_t argNo, uint8_t order, uint16_t _enum){
	op_struct *out = malloc(sizeof(*out));
	if(!out)
		return NULL;

	out->order = order;
	out->argNo = argNo;
	out->_enum = _enum;

	return out;
}


stack *new_stk(int size) {
	stack *out = malloc(sizeof(*out));
	if(!out)
		return NULL;

	//allocate pointers
	out->stk = malloc(sizeof(*out->stk) * size);
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
