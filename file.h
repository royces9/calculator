#ifndef FILES
#define FILES

#include "fileStruct.h"

int runFile(char **input, vari *var);
fileStack newFileStack();
int createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize);
int executeTree(fileTree *tree, vari *var, int maxSize); 
int checkProgramFlow(char *input);
int checkConditional(char *input, int type, vari *var);
void freeString(char **string, int max);

#endif //FILES
