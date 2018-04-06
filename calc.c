#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "stack.h"
#include "sya.h"

int main(int argc, char *argv[]) {
  char *input = NULL;
  int error = 0;
  double ans = 0; //stores the previous answer in this variable, can be used by using "ans"

  //initialize variable struct
  vari var = newVari();

  //execute command line arguments first
  if(argc > 1) {
    for(int i = 1; i < argc; ++i){
      printf(">>%s\n", *(argv+i));
      error = sya(argv[i], &ans, &var);

      if(error == 0) {
	printf("\n%lf\n\n",ans);
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
      error = sya(input, &ans, &var);
    }

    if(error == 0) {
      printf("\n%lf\n\n", ans);
    } else{ //if the error is less than -1, prints and error code
      errorrep(error);
    }

    free(input); //readline mallocs the input line
  }
  return 0;
}
