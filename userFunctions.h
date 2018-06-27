#ifndef USER_FUNCTIONS
#define USER_FUNCTIONS

matrix *findUserFunction(char *functionName, char **functionArgs, vari *var, error_return *error);
char *findFunctionPath(char *functionName, error_return *error);
matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, error_return *error);

#endif //USER_FUNCTIONS
