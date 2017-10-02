#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "stack.h"
#include "sya.h"

int main(int argc, char *argv[]){
  char *input = NULL;
  int error = 0;
  double ans = 0;

  vari var;

  var.count = 0;
  var.occ = 0;
  memset(var.name, '\0', sizeof(var.name));
  memset(var.value, 0, sizeof(var.value));
  
  if(argc > 1){
    for(int i = 1; i < argc; i++){
      printf(">>%s\n", *(argv+i));
      error = sya(argv[i], &ans, &var);

      if(error == 0){
	printf("\n%lf\n\n",ans);
      }
      else{
	errorrep(error);
      }
    }
  } 
  while(error <= 0){
    input = readline(">>");
    add_history(input);

    if(*input == 0){
      continue;
    }
    
    else{
      error = sya(input, &ans, &var);
    }
    if(error == 0){
      printf("\n%lf\n\n", ans);
    }
    else{
      errorrep(error);
    }

    free(input); //readline mallocs the input line
  }
  return 0;
}
