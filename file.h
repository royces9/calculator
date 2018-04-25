#ifndef FILES
#define FILES

#include "fileStruct.h"

int runFile(char **input, vari *var, matrix *ans);
fileStack newFileStack();
int createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize);
int executeTree(fileTree *tree, vari *var, matrix *ans, int maxSize); 
int checkProgramFlow(char *input);
int checkConditional(char *input, int type, vari *var, matrix *ans);
void freeString(char **string, int max);

#endif //FILES
