#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "types.h"
#include "matrix.h"
#include "variables.h"
#include "stack.h"

#include "sya.h"

int main(int argc, char *argv[]) {
	char *input = NULL;
	err_ret error = 0;
  
	//initialize variable struct
	vari *var = init_var(256);

	//init random seed
	srand(time(0));

	//execute command line arguments first
	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			printf(">>%s\n", argv[i]);
			error = sya(argv[i], var);

			if( !error ) {
				print_mat(var->ans);

			} else {
				err_rep(error);

			}
		}
	} 

	//main loop
	while(error <= 0) {
		error = 0;

		//user input and history
		input = readline(">>");
		add_history(input);

		//break on EOF
		//skip empty lines
		if( !input ) {
			printf("\n");
			break;
		} else if( (*input) ) {
			//parses string and does all the calculations
			error = sya(input, var);

			if( !error ) {
				//suppress output if the line ends with ';'
				if( input[strlen(input) - 1] != ';' )
					print_mat(var->ans);

			} else { //if the error is less than -1, prints an error code
				err_rep(error);
			}
		}

		//readline mallocs the input line
		free(input);
	}

	free_var(var);
	return 0;
}
