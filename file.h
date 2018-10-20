#ifndef FILES
#define FILES

#include "fileStruct.h"

err_ret runFile(char **input, vari *var, int offset);
err_ret createTree(char *fileName, fileTree *tree, int skip);
err_ret executeTree(fileTree *tree, vari *var); 
err_ret checkProgramFlow(char *input);
err_ret checkConditional(char *input, int type, vari *var);

#endif //FILES
