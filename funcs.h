#ifndef FUNCS
#define FUNCS

operatorStruct setOpStack(char oeprator, int argNo);
double op(double a, double b, char o);
double ops(double a, char o);
double factorial(double a, int *error);
void exec_num(numberStack *num, operatorStruct ch);
int funcfind(char buffer[]);
int charfind(char buffer[], numberStack *num, operatorStack *ch, double ans, vari *var, int *tok, int *start, char input[]);
int varcheck(vari *list, char inp[]);

#endif //FUNCS
