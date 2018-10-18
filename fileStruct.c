#include <stdlib.h>

#include "types.h"

#include "fileStruct.h"


fileTree *createLeaf(void) {
	return calloc(1, sizeof(fileTree));
}

void cutDownTree(fileTree *tree) {
	if(tree) {
		tree->line = NULL;
		cutDownTree(tree->left);
		cutDownTree(tree->right);

		free(tree);
	}
}
