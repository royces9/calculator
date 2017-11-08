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
  int left = 0, right = 0;
  if(tree->left == NULL) left = 1;
  if(tree->right == NULL) right = 2;
  switch(left + right){
  case 0: free(tree); break;
  case 1: cutDownTree(tree->left); break;
  case 2: cutDownTree(tree->right); break;
  case 3: cutDownTree(tree->left); cutDownTree(tree->right); break;
  }
  free(tree);
}



void fPush(fileStack *stk, fileTree *node){
  stk->stk[stk->top++] = node;
}

 fileTree *fPop(fileStack *stk){
  return stk->stk[--stk->top];
}
