#ifndef FILESTRUCT
#define FILESTRUCT

typedef struct fileTree{
  char *line;
  struct fileTree *left;
  struct fileTree *right;
} fileTree;

typedef struct{
  fileTree *stk[1024];
  int top;
  int occ;
} fileStack;

fileTree *createLeaf();
void cutDownTree(fileTree *tree);
void fPush(fileStack *stk, fileTree *node);
fileTree *fPop(fileStack *stk);

#endif //FILESTRUCT
