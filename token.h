#ifndef TOKEN
#define TOKEN

typedef struct {
	char *tok;
	unsigned int len;
	int type;
	unsigned int arg;
} token;


typedef struct tok_tree {
	token *leaf;
	struct tok_tree **children;
} tok_tree;

token **tokenize(char *inp);

tok_tree *make_tok_tree(token **tok_list);
int tok_chk_num(token *tok);
#endif //TOKEN
