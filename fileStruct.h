#ifndef FILESTRUCT
#define FILESTRUCT

typedef struct fileTree {
	char *line;
	struct fileTree *left;
	struct fileTree *right;
	int num;
} fileTree;

typedef struct {
	int top;
	fileTree *stk[1024];
	int occ;
} fileStack;

fileTree *createLeaf();
void cutDownTree(fileTree *tree);
void fPush(fileStack *stk, fileTree *node);
fileTree *fPop(fileStack *stk);

#endif //FILESTRUCT
