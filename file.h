#ifndef FILES
#define FILES

#include "fileStruct.h"

err_ret runFile(char **input, struct vari *var, int offset);
err_ret createTree(char *fileName, fileTree *tree, int skip);
err_ret executeTree(fileTree *tree, struct vari *var); 
int checkProgramFlow(char *input);
err_ret checkConditional(char *input, int type, struct vari *var);

#endif //FILES
