#ifndef USER_FUNCTIONS
#define USER_FUNCTIONS

matrix *findUserFunction(char *functionName, char **functionArgs, vari *var, err_ret *error);
char *checkConfig(char *functionName, char *configPath, err_ret *error);
char *findFunctionPath(char *functionName, err_ret *error);
matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, err_ret *error);
uint8_t checkFunctionName(char *functionName, char *directory);

#endif //USER_FUNCTIONS
