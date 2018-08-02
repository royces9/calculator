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
	error_return error = 0;
  
	//initialize variable struct
	vari *var = newVari();

	//init random seed
	srand(time(0));

	//execute command line arguments first
	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			printf(">>%s\n", argv[i]);
			error = sya(argv[i], var);

			if( !error ) {
				printMatrix(var->ans);

			} else {
				errorReport(error);

			}
		}
	} 

	//main loop
	while(error <= 0) {

		error = 0;

		//user input and history
		input = readline(">>");
		add_history(input);

		//skip empty lines
		if( (*input) ) {
			//parses string and does all the calculations
			error = sya(input, var);

			if( !error ) {

				//suppress output if the line ends with ';'
				if( input[strlen(input) - 1] != ';' )
					printMatrix(var->ans);

			} else { //if the error is less than -1, prints an error code
				errorReport(error);
			}
		}

		//readline mallocs the input line
		free(input);
		input = NULL;
	}

	freeVari(var);
	return 0;
}
