#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "sya.h"
#include "token.h"
#include "operator.h"
#include "operatorUtility.h"

token **tokenize(char *inp) {
	int len = strlen(inp);
	token **out = malloc(sizeof(*out) * (len + 1));
	char *type = calloc(sizeof(*type), (len + 1));

        for(int l = 0; inp[l]; ++l) {
		if( (type[l] = chk_t(inp[l])) == 3) {
			type[l + 1] = chk_t(inp[l + 1]);

			type[l] = (type[l + 1] == 2) ? type[l + 1] : 1;
			++l;
		}
	}

	int tok_count = 0;
	token *cur = malloc(sizeof(*cur));

	for(int i = 0, j = 1; inp[i]; i = j, j = i + 1) {
		switch(type[i]) {
		case 1:
			while(type[i] == type[j])
				++j;

			cur->arg = -1;
			break;

		case 2:
			while(type[i] == type[j]) {
				char cpy[16];
				memset(cpy, 0, 16);
				strncpy(cpy, inp + i, j - i + 1);
				if(OPERATOR_COUNT == search_str(cpy, OPERATOR_LIST))
					break;

				++j;
			}

			cur->arg = -1;
			break;
			
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			cur->arg = 0;
			break;
			
		default:
			++i;
			continue;
		}

		cur->tok = inp + i;
		cur->len = j - i;
		cur->type = type[i];

		out[tok_count++] = cur;
		cur = malloc(sizeof(*cur));
	}
	out[tok_count] = NULL;
	free(type);

	return out;
}


/*
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
		return 4;
	case ']':
		return 5;

	case '(':
		return 6;
	case ')':
		return 7;

	case ';':
	case ',':
		return 8;


	case '\t':
	case '\n':
		return -2;

		
	case ' ':
		return 0;

	default:
		return -1;
	}
}
*/

tok_tree *make_tok_tree(token **tok_list) {
	tok_tree *out = malloc(sizeof(*out));

	struct stack *num = new_stk(512);
	struct stack *op = new_stk(512);

	for(int i = 0; tok_list[i]; ++i) {
		token *cur = tok_list[i];

		switch(cur->type) {
		case 1: //alphanumerics
			if(tok_chk_num(cur)) {
				push(num, cur);
			} else {
			}
			break;
			
		case 2: //operator
			break;
			
		case 3: //( [
		case 5:
			push(op, cur);
			break;

		case 4: //) ]
		case 6:
			do {
				push(num, pop(op));
			} while((op->top > -1) && ( *(((token *)op->stk[op->top]))->tok) != '(');

			pop(op);
			break;


		}
	}

	return out;
}

int tok_chk_num(token *tok) {
	for(int i = 0; i < tok->len; ++i) {
		if( ((tok->tok[i] <'0') && (tok->tok[i] != '.')) ||
		    (tok->tok[i] > '9') ||
		    (!tok->tok[i]) )
			return 0;

	}

	return 1;
}

