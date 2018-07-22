#ifndef OPERATOR
#define OPERATOR


int searchFunctionArray(char *buffer);
int searchOperatorArray(char *buffer);
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
error_return execNum(numberStack *num, vari *var, operatorStruct *ch);

matrix *matrixOneArg(matrix *a, operatorStruct *ch, error_return *error);
matrix *matrixTwoArg(matrix *a, matrix *b, operatorStruct *ch, error_return *error);

error_return findFunction(char *buffer, numberStack *num, operatorStack *ch, vari *var, int8_t *tok, uint16_t *iterator, char *input);
error_return findOperator(char *buffer, numberStack *num, operatorStack *ch, vari *var, int8_t *tok);

char **separateMatrix(char *input, int length, error_return *error);
matrix *extractMatrix(vari *var, uint16_t *iterator, char *input, error_return *error);

void helpPrint(void);

#endif //OPERATOR
