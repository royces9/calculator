#ifndef STACK
#define STACK

typedef struct {
	int top;
	void *stk[1024];
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

} operatorStruct;


typedef struct { //struct for stack of numbers
	//index for the top index
	//-1 is empty
	int top;

	//stack array
	matrix *stk[1024];
} numberStack;


typedef struct { //same as numberStack, except for operators
	//index for the top index
	//-1 is empty
	int top;

	//stack array
	operatorStruct *stk[1024];
} operatorStack;

void push(void *d, void *stk);
void *pop(void *stk);

void pushn(matrix *inp, numberStack *st);
matrix *popn(numberStack *st);

void pushch(operatorStruct *inp, operatorStack *st);
operatorStruct *popch(operatorStack *st);

operatorStruct *init_op_struct(const char *op, uint8_t argNo, uint8_t order, uint8_t _enum);

numberStack *newNumberStack(void);
operatorStack *newOperatorStack(void);

void freeNumberStack(numberStack *st);
void freeOperatorStack(operatorStack *st);

#endif //STACK
