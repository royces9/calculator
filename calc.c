#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "strucon.h"
#include "sya.h"
#include "stack.h"


//06/15/2017

int main(int argc, char *argv[]){
  char *input;
  int error = 0;
  double ans = 0;
  vari var;

  var.count = 0;
  var.occ = 0;
  if(argc > 1){
    printf(">>%s\n", *(argv+1));
    error = sya(argv[1], &ans, &var);
  }
  if(error == 0){
    printf("\n%lf\n\n",ans);
  }
  else{
    errorrep(error);
  }
   
  while(error <= 100){
    input = readline(">>");
    add_history(input);
    
    if( *input == 0){
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
  }
  return 0;
}
