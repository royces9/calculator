#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var, double *ans){
  FILE *inputFile;
  char buffer[1024];
  int error = 0, length = 0;

  inputFile = fopen(input[0], "r");
  if(!inputFile){
    return error = -7;
  }
  printf("\n");

  while(fgets(buffer, 1024, inputFile)){

    length = strlen(buffer);

    if(!strcmp(buffer, "\n")){
      continue;
    }

    else{
      if(buffer[0] == '#'){ //# comments out a line
	continue;
      }

      if(buffer[length - 1] == '\n'){
	buffer[length - 1] = '\0';
	length=strlen(buffer);
      }

      if(buffer[length - 1] != ';'){
	printf("> %s\n", buffer);
      }
      error = sya(buffer, ans, var);

      if(error){
	fclose(inputFile);
	return error;
      }

      if(buffer[length - 1] != ';'){
	printf(">    %lf\n\n", *ans);
      }
    }
  }  
  fclose(inputFile);
  return 0;
}
