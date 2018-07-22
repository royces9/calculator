#ifndef STACK
#define STACK

typedef struct { //struct for operators, +, -, etc
	//operator/function string
	char operator[16];

	//order of operation, higher is higher priority
	uint8_t precedence;

	//number of arguments it takes, generally two or one
	uint8_t argNo;

	//corresponding enum, in operatorUtility.h
	uint8_t enumeration;

} operatorStruct;


typedef struct { //struct for stack of numbers
	//stack array
	matrix *stk[1024];

	//index for the top index
	//-1 is empty
	int top;

} numberStack;


typedef struct { //same as numberStack, except for operators
	//stack array
	operatorStruct *stk[1024];

	//index for the top index
	//-1 is empty
	int top;

} operatorStack;

void pushn(matrix *inp, numberStack *st);
matrix *popn(numberStack *st);

void pushch(operatorStruct *inp, operatorStack *st);
operatorStruct *popch(operatorStack *st);

operatorStruct *initOperatorStruct(const char *operator, uint8_t argNo, uint8_t precedence, uint8_t enumeration);

numberStack *newNumberStack(void);
operatorStack *newOperatorStack(void);

void freeNumberStack(numberStack *st);
void freeOperatorStack(operatorStack *st);

#endif //STACK
