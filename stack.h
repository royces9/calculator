#ifndef STACK
#define STACK

struct stack {
	//stack, elements must be kept as pointers
	void **stk;

	//index to top of stack
	//-1 is empty
	int top;
};

/*
 * push and pop members of stack
 * they both change the top member in stack
 * pop returns the popped value
 */
void push(struct stack *stk, void *d);
void *pop(struct stack *stk);

/*
 * initialize new stack
 * size is the maximum number of elements
 */
struct stack *new_stk(int size);

/*
 * frees stack and all members
 * stk - pointer to the stack
 * free_fun - function pointer to a free function
 *            for every member of the stack
 *            does nothing if NULL
 */
void free_stk(struct stack *stk, void (*free_fun)(void *));

/*
 * returns pointer to the top member of the stack
 */
void *top_stk(struct stack *stk);

#endif //STACK
