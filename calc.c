#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "stack.h"
#include "sya.h"

int main(int argc, char *argv[]) {
  char *input = NULL;
  int error = 0;

  //stores the previous answer in this variable, can be used by using "ans"
  /*
  matrix ans;
  ans.size = NULL;
  ans.elements = NULL;
  */
  
  //initialize variable struct
  vari var = newVari();

  //execute command line arguments first
  if(argc > 1) {
    for(int i = 1; i < argc; ++i){
      printf(">>%s\n", *(argv+i));
      error = sya(argv[i], &var);

      if(error == 0) {
	printMatrix(&var.ans);
      } else{
	errorrep(error);
      }
    }
  } 

  //main loop
  while(error <= 0) {
    //user input and history
    input = readline(">>");
    add_history(input);

    if(*input == 0) { //if the user enters an empty line, go to top of loop
      free(input);
      continue;
    } else{ //parses string and does all the calculations
      error = sya(input, &var);
    }

    if(error == 0) {
      printMatrix(&var.ans);
    } else{ //if the error is less than -1, prints an error code
      errorrep(error);
    }
    free(input); //readline mallocs the input line
  }

  freeVari(&var);
  return 0;
}
