#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

token **tokenize(char *inp) {
	int len = strlen(inp);
	token **out = malloc(sizeof(*out) * len);

	char *type = malloc(sizeof(*type) * (len + 1));

        for(int l = 0; inp[l]; ++l) {
		if( (type[l] = chk_tt(inp[l])) == 3) {

			if( (type[l + 1] = chk_tt(inp[l + 1])) == 2) {
				type[l] = type[l + 1];
			} else {
				type[l] = 1;
			}

			++l;
		}
	}

	int i = 0;
	int tok_count = 0;
	token *cur = malloc(sizeof(*cur));

	while(inp[i]) {
		switch(type[i]) {
		case 1:
		case 2:
			;
			int j = i;
			while(type[i] == type[j])
				++j;

			cur->tok = inp + i;
			cur->len = j - i;
			cur->type = type[i];
			cur->arg = -1;

			i = j;
			break;

		case 4:
		case 5:
		case 6:
			cur->tok = inp + i;
			cur->len = 1;
			cur->type = type[i];
			cur->arg = 0;
			++i;
			break;

		default:
			++i;
			continue;

		}

		out[tok_count++] = cur;
		cur = malloc(sizeof(*cur));
	}
	out[tok_count] = NULL;

	return out;
}


char chk_tt(char a) {
	switch(a) {

	case '0' ... '9':
	case 'a' ... 'z':
	case 'A' ... 'Z':
	case '_':
		return 1;


	case '^':
	case '*':
	case '/':
	case '+':
	case '=':
	case '>':
	case '<':
	case '!':
	case '&':
	case '|':
	case '~':
	case '%':
	case '-':
		return 2;


	case '.':
		return 3;


	case '[':
	case ']':
		return 4;


	case '(':
	case ')':
		return 5;


	case ';':
	case ',':
		return 6;


	case '\t':
	case '\n':
		return -2;

		
	case ' ':
		return 0;

	default:
		return -1;
	}
}


tok_tree *make_tok_tree(token **tok_list) {
	tok_tree *out = NULL;
}
