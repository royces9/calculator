#ifndef STACK
#define STACK

typedef struct {
	//stack, elements must be kept as pointers
	void **stk;

	//index to top of stack
	//-1 is empty
	int top;
} stack;

typedef struct { //struct for operators, +, -, etc
	//order of operation, higher is higher priority
	uint8_t order;

	//number of arguments it takes, generally two or one
	uint8_t argNo;

	//corresponding enum, in operatorUtility.h
	uint16_t _enum;

	//function pointer
	void *fp;
} op_struct;

/*
 * push and pop members of stack
 * they both change the top member in stack
 * pop returns the popped value
 */
void push(stack *stk, void *d);
void *pop(stack *stk);

/*
 * initialize operator struct
 */
op_struct *init_op_struct(uint8_t argNo, uint8_t order, uint16_t _enum);

/*
 * initialize new stack
 * size is the maximum number of elements
 */
stack *new_stk(int size);

/*
 * stk - pointer to the stack
 * free_fun - function pointer to a free function
 *            for every member of the stack
 *            does nothing if NULL
 */
void free_stk(stack *stk, void (*free_fun)(void *));

#endif //STACK
