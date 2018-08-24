#ifndef USER_FUNCTIONS
#define USER_FUNCTIONS

matrix *findUserFunction(char *functionName, char **functionArgs, vari *var, error_return *error);
char *checkConfig(char *functionName, char *configPath, error_return *error);
char *findFunctionPath(char *functionName, error_return *error);
matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, error_return *error);
uint8_t checkFunctionName(char *functionName, char *directory);

#endif //USER_FUNCTIONS
