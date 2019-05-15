#ifndef STACK
#define STACK

struct stack {
	//stack, elements must be kept as pointers
	void **stk;

	//index to top of stack
	//-1 is empty
	int top;
};

struct matrix;
struct vari;

union fp {
	struct matrix * (*mult)(char **, struct vari *, err_ret *);
	struct matrix * (*m_one)(struct matrix *, err_ret *);
	struct matrix * (*m_two)(struct matrix *, struct matrix *, err_ret *);
	ele (*s_one)(ele, err_ret *);
	ele (*s_two)(ele, ele);
	void *p;
};


struct oper{ //struct for operators, +, -, etc
	//function pointer
	union fp fp;
	
	int _enum;

	//order of operation, higher is higher priority
	uint8_t order;

	//number of arguments it takes, generally two or one
	uint8_t argNo;

	//1 if matrix operator, 0 if scalar
	uint8_t mat_op;
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
 * stk - pointer to the stack
 * free_fun - function pointer to a free function
 *            for every member of the stack
 *            does nothing if NULL
 */
void free_stk(struct stack *stk, void (*free_fun)(void *));
void *top_stk(struct stack *stk);

#endif //STACK
