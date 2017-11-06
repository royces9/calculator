#ifndef FILESTRUCT
#define FILESTRUCT

typedef struct{
  char *line;
  fileTree *left;
  fileTree *right;
} fileTree;

typedef struct{
  char type;
  fileTree *stk[1024];
  int top;
} fileStack;

fileTree *createLeaf();

void fPush(fileStack *stk, fileTree *node);
fileTree fPop(fileStack *stk);


#endif //FILESTRUCT
