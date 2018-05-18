#ifndef OPERATOR
#define OPERATOR

#include "stack.h"
#include "operatorUtility.h"

int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
void execNum(numberStack *num, operatorStruct ch, error_return *error);

matrix *matrixOneArg(matrix *a, operatorStruct ch, error_return *error);
matrix *matrixTwoArg(matrix *a, matrix *b, operatorStruct ch, error_return *error);

error_return findFunction(char *buffer, numberStack *num, operatorStack *ch, vari *var, int *tok, int *start, char *input);
error_return findOperator(char *buffer, numberStack *num, operatorStack *ch, vari *var, int *tok);

char **separateMatrix(char *input, int length, error_return *error);
matrix *extractMatrix(vari *var, int *start, char *input, error_return *error);

int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);
void helpPrint(void);

#endif //OPERATOR
