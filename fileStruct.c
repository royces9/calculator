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

void fPush(fileStack *stk, fileTree *node) {
	if(stk->occ == 1) {
		stk->stk[++stk->top] = node;
	} else {
		stk->stk[0] = node;
		stk->occ = 1;
	}
}

fileTree *fPop(fileStack *stk) {
	fileTree *out = NULL;

	if(stk->occ == 1) {
		out = stk->stk[stk->top--];
		if(stk->top == -1) {
			stk->occ = 0;
			stk->top = 0;
		}
	}

	return out;
}
