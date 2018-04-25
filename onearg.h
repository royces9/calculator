#ifndef ONEARG
#define ONEARG

int findFunction(char *buffer, numberStack *num, operatorStack *ch, matrix *ans, vari *var, int *tok, int *start, char input[]);
int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);
void helpPrint();
#endif //ONEARG
