#ifndef FUNCS
#define FUNCS

#define __MALLOC_CHECK(pointer, error)	\
  if(pointer == NULL){\
  error = -6; \
  return 0; \
  }

operatorStruct setOpStack(const char *operator, int argNo, int precedence);
double op(double a, double b, char o);
double ops(double a, char o);
double factorial(double a, int *error);
void execNum(numberStack *num, operatorStruct ch);
int findFunction(char *buffer, numberStack *num, operatorStack *ch, double ans, vari *var, int *tok, int *start, char input[]);
int varcheck(vari *list, char inp[]);
void freeDoubleArray(char **input);
void helpPrint();
#endif //FUNCS
