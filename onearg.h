#ifndef ONEARG
#define ONEARG

void execNum(numberStack *num, operatorStruct ch);
int findFunction(char *buffer, numberStack *num, operatorStack *ch, double ans, vari *var, int *tok, int *start, char input[]);
int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);
void helpPrint();
#endif //ONEARG
