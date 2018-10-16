#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#include "fileStruct.h"


fileTree *createLeaf(err_ret *error) {
	fileTree *leaf = calloc(1, sizeof(*leaf));
	__MALLOC_CHECK(leaf, *error);

	return leaf;
}

void cutDownTree(fileTree *tree) {
	if(tree) {
		tree->line = NULL;
		cutDownTree(tree->left);
		cutDownTree(tree->right);

		free(tree);
	}
}
