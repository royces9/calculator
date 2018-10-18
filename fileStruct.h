#ifndef FILESTRUCT
#define FILESTRUCT

typedef struct fileTree {
	char *line;
	struct fileTree *left;
	struct fileTree *right;
	int num;
} fileTree;


fileTree *createLeaf(void);
void cutDownTree(fileTree *tree);

#endif //FILESTRUCT
