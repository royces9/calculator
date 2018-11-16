#ifndef TOKEN
#define TOKEN

typedef struct {
	char *tok;
	unsigned int len;
	int type;
	unsigned int arg;
} token;


typedef struct {
	token *leaf;
	token **children;
} tok_tree;

token **tokenize(char *inp);
static int8_t chk_t(char a);

#endif //TOKEN
