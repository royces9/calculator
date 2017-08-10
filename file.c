#include <stdio.h>

#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var){
  FILE *inputFile;
  char buffer[1024];
  double ans;
  int error;

  inputFile = fopen(input[0], "r");
  if(!inputFile){
    return error = -7;
  }
  while(fgets(buffer, 1024, inputFile)){
    error = sya(buffer, &ans, var);
    if(error){
      fclose(inputFile);
      return error;
    }

  }

  fclose(inputFile);
  return 0;
}
