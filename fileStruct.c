#include <stdlib.h>

#include "types.h"

#include "fileStruct.h"


fileTree *new_leaf(void) {
	return calloc(1, sizeof(fileTree));
}


void cutDownTree(fileTree *tree) {
	if(tree) {
		free(tree->line);
		cutDownTree(tree->left);
		cutDownTree(tree->right);

		free(tree);
	}
}
