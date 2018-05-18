#ifndef FILES
#define FILES

#include "fileStruct.h"

error_return runFile(char **input, vari *var);
fileStack newFileStack();
error_return createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize);
error_return executeTree(fileTree *tree, vari *var, int maxSize); 
error_return checkProgramFlow(char *input);
error_return checkConditional(char *input, int type, vari *var);
void freeString(char **string, int max);

#endif //FILES
