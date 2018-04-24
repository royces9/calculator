#ifndef FILES
#define FILES

#include "fileStruct.h"

int runFile(char **input, vari *var, double *ans);
fileStack newFileStack();
int createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize);
int executeTree(fileTree *tree, vari *var, double *ans, int maxSize); 
int checkProgramFlow(char *input);
int checkConditional(char *input, int type, vari *var, double *ans);
void freeString(char **string, int max);

#endif //FILES
