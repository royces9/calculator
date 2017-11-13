#ifndef FILES
#define FILES

int runFile(char **input, vari *var, double *ans);
int checkProgramFlow(char *input);
int checkConditional(char *input, int type, vari *var, double *ans);
void freeString(char **string, int max);
#endif //FILES
