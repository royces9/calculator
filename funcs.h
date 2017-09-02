#ifndef FUNCS
#define FUNCS

#define __MALLOC_CHECK(pointer, error)	\
  if(pointer == NULL){\
  error = -6; \
  return 0; \
  }

operatorStruct setOpStack(char oeprator, int argNo);
double op(double a, double b, char o);
double ops(double a, char o);
double factorial(double a, int *error);
void exec_num(numberStack *num, operatorStruct ch);
int funcfind(char buffer[]);
int charfind(char buffer[], numberStack *num, operatorStack *ch, double ans, vari *var, int *tok, int *start, char input[]);
int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);

#endif //FUNCS
