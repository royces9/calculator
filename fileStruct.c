#include "fileStruct.h"

fileTree *createLeaf(){
  fileTree *file = malloc(sizeof(fileTree));
  return file;
}

void fPush(fileStack *stk, fileTree *node){
  stk->stk[top++] = node;
}

fileTree *fPop(fileStack *stk){
  return stk->stk[top--] = node;  
}
