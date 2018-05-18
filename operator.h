#ifndef OPERATOR
#define OPERATOR

#include "stack.h"
#include "operatorUtility.h"

int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
void execNum(numberStack *num, operatorStruct ch, int8_t *error);

matrix *matrixOneArg(matrix *a, operatorStruct ch, int8_t *error);
matrix *matrixTwoArg(matrix *a, matrix *b, operatorStruct ch, int8_t *error);

int8_t findFunction(char *buffer, numberStack *num, operatorStack *ch, vari *var, int *tok, int *start, char *input);
int8_t findOperator(char *buffer, numberStack *num, operatorStack *ch, vari *var, int *tok);

char **separateMatrix(char *input, int length, int8_t *error);
matrix *extractMatrix(vari *var, int *start, char *input, int8_t *error);

int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);
void helpPrint();

#endif //OPERATOR
