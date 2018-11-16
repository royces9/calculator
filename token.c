#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

token **tokenize(char *inp) {
	int len = strlen(inp);
	token **out = malloc(sizeof(*out) * len);

	for(int i = 0; inp[i]; ++i) {
		char type = chk_t(inp[i]);
		token *cur = malloc(sizeof(*cur));
		switch(type) {
		case 1:
			break;

		case 2:
			break;

		case 3:
			break;
			
		case 4:
			;
			//start at the first char after '['
			int j = i + 1;
			//put everything enclosed in [] in a single token
			for(int count = 1; count && inp[j]; ++j) {
				if(inp[j] == '[')
					++count;

				if(inp[j] == ']')
					--count;
			}

			cur->tok = inp[i];
			cur->len = j - i + 1;
			cur->type = 2;
			cur->arg = 0;

			//set i to the final ']'
			i = j;
			break;

		case 5:
			break;

		default:
			break;
		}
	}

	return out;
}


int8_t chk_t(char a) {
	switch(a) {

	case '0' ... '9':
	case 'a' ... 'z':
	case 'A' ... 'Z':
	case '_':
		return 1;

	case '^':
	case '(':
	case '*':
	case '/':
	case '+':
	case ')':
	case '=':
	case '>':
	case '<':
	case '!':
	case '&':
	case '|':
	case '~':
	case '%':
		return 2;

	case '.':
		return 3;

	case '[':
	case ']':
		return 4;

	case '-':
		return 5;

	case '\t':
	case '\n':
	case ';':
	case ' ':
		return 0;

	default:
		return -1;
	}
}
