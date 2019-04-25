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
char chk_tt(char a);

tok_tree *make_tok_tree(token **tok_list);
int chk_numm(token *tok);
#endif //TOKEN
