#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

#include "userFunctions.h"
#include "matrix.h"
#include "multi.h"

matrix *findUserFunction(char *functionName, char **functionArgs, vari *var, error_return *error){
  struct direct *d;
  DIR *currentDirectory = opendir(".");

  uint8_t foundFlag = 0;
  while((d = readdir(dir)) != NULL){
    int length = strlen(d->d_name);

    //checks that function name is the same, and ends in ".cr"
    if(!memcmp(d->d_name, functionName, length - 3) &&
       !strcmp(d->d_name + (length - 3), ".cr")){

      foundFlag = 1;
      break;

    }
  }

  if(!foundFlag){
    *error = -5;
    return NULL;
  }


  matrix *out = executeUserFunction();

  return out;  
}
