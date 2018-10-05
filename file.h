#ifndef FILES
#define FILES

#include "fileStruct.h"

err_ret runFile(char **input, vari *var, int offset);
fileStack newFileStack();
err_ret createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize, int offset);
err_ret executeTree(fileTree *tree, vari *var, int maxSize); 
err_ret checkProgramFlow(char *input);
err_ret checkConditional(char *input, int type, vari *var);
void freeString(char **string, int max);

#endif //FILES
