#ifndef FILES
#define FILES

#include "fileStruct.h"

int8_t runFile(char **input, vari *var);
fileStack newFileStack();
int8_t createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize);
int8_t executeTree(fileTree *tree, vari *var, int maxSize); 
int8_t checkProgramFlow(char *input);
int8_t checkConditional(char *input, int type, vari *var);
void freeString(char **string, int max);

#endif //FILES
