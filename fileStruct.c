#include <stdio.h>
#include <stdlib.h>
#include "fileStruct.h"

fileTree *createLeaf(){
  fileTree *file = malloc(sizeof(fileTree));
  if(file == NULL){
    printf("NULL\n");
    return NULL;
  }
  file->line = NULL;
  file->left = NULL;
  file->right = NULL;
  return file;
}

void cutDownTree(fileTree *tree){
}


void fPush(fileStack *stk, fileTree *node){
  stk->stk[stk->top++] = node;
}

 fileTree *fPop(fileStack *stk){
  return stk->stk[--stk->top];
}
