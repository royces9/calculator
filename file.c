#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var, double *ans){
  FILE *inputFile;
  char buffer[1024];
  int error;

  inputFile = fopen(input[0], "r");
  if(!inputFile){
    return error = -7;
  }

  printf("\n");
  while(fgets(buffer, 1024, inputFile)){
    for(int i = 0; buffer[i]; i++){
      if(buffer[i] == ';'){
	buffer[i+1] = '\0';
      }
    }
    printf("> %s\n", buffer);
    error = sya(buffer, ans, var);
    printf(">    %lf\n\n", *ans);
    if(error){
      fclose(inputFile);
      return error;
    }

  }
  fclose(inputFile);
  return 0;
}
