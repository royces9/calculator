#ifndef STACK
#define STACK

typedef struct {

	void **stk;

	//index to top of stack
	//-1 is empty
	int top;
} stack;

typedef struct { //struct for operators, +, -, etc
	//operator/function string
	char op[16];

	//order of operation, higher is higher priority
	uint8_t order;

	//number of arguments it takes, generally two or one
	uint8_t argNo;

	//corresponding enum, in operatorUtility.h
	uint8_t _enum;

} op_struct;

void push(stack *stk, void *d);
void *pop(stack *stk);

op_struct *init_op_struct(const char *op, uint8_t argNo, uint8_t order, uint8_t _enum);

void *new_stk(int size);
void free_stk(stack *stk, void (*free_fun)(void *));

#endif //STACK
