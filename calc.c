#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "strucon.h"
#include "sya.h"

#include "stack.h"
#include "funcs.h"

//06/15/2017

int main(int argc, char* argv){
  char* input;
  int error = 0;
  double ans = 0;
  vari var;

  var.count = 0;
  var.occ = 0;
  
  while(error <= 100){
    input = readline(">>");
    add_history(input);
    
    if( *input == 0){
      continue;
    }
    
    else{
      error = sya(input, &ans, &var);
    }

    errorrep(error);
  }
  return 0;
}
