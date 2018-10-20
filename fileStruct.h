#ifndef FILESTRUCT
#define FILESTRUCT

typedef struct fileTree {
	char *line;
	struct fileTree *left;
	struct fileTree *right;
	int num;
} fileTree;


fileTree *new_leaf(void);
void cutDownTree(fileTree *tree);

#endif //FILESTRUCT
