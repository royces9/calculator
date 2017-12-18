#ifndef FILES
#define FILES

#include "fileStruct.h"

int runFile(char **input, vari *var, double *ans);
fileStack newFileStack();
int checkProgramFlow(char *input);
int checkConditional(char *input, int type, vari *var, double *ans);
void freeString(char **string, int max);
#endif //FILES
