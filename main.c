#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "types.h"

#include "token.h"
#include "sya.h"

int main(int argc, char *argv[]) {
	char *input = NULL;

	struct vari *var = NULL;
	//initialize variable struct
	err_ret err = init_var(256, &var);
	if(err) {
		err_rep(err);
		return -1;
	}
	
	//init random seed
	srand(time(0));

	//execute command line arguments first
	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			printf(">>%s\n", argv[i]);
			
			err = sya(argv[i], var);

			if(!err)
				print_mat(var->ans);
			else
				err_rep(err);
		}
	} 

	//main loop
	while(err != e_exit) {
		err = e_ok;

		//user input and history
		input = readline(">>");
		add_history(input);

		/*
		token **tok_list = tokenize(input);

		char cpy[100];
		for(int i = 0; tok_list[i]; ++i) {
			memset(cpy, 0, 100);
			strncpy(cpy, tok_list[i]->tok, tok_list[i]->len);
			printf("%d: ", i);
			puts(cpy);
		}

		tok_tree *tree = make_tok_tree(tok_list);
		*/
		//break on EOF
		//skip empty lines
		if( !input ) {
			puts("");
			break;
		} else if( *input ) {
			//parses string and does all the calculations
			err = sya(input, var);
			if(!err) {
				//suppress output if the line ends with ';'
				if( input[strlen(input) - 1] != ';' )
					print_mat(var->ans);

			} else { //if the error is less than -1, prints an error code
				err_rep(err);
			}
		}

		//readline mallocs the input line
		free(input);
	}

	free_var(var);
	return 0;
}
