#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

#include "stack.h"
#include "multi.h"
#include "userFunctions.h"

char *configFilePath = "~/.calc.cfg";

matrix *findUserFunction(char *functionName, char **functionArgs, vari *var, error_return *error){

  char *functionPath = findFunctionPath(functionName);
  if(!functionPath){
    *error = -5;
    return NULL;
  }


  matrix *out = executeUserFunction(functionPath, functionArgs, var, error);

  free(functionPath);

  return out;  
}


//returns the path to the function
char *findFunctionPath(char *functionName){
  struct dirent *d;
  DIR *currentDirectory = opendir(".");

  uint8_t foundFlag = 0;

  int length = 0;

  char *out = NULL;

  //checks the current directory
  while((d = readdir(currentDirectory)) != NULL){
    length = strlen(d->d_name);

    //checks that function name is the same, and ends in ".cr"
    if(!memcmp(d->d_name, functionName, length - 3) &&
       !strcmp(d->d_name + (length - 3), ".cr")){

      foundFlag = 1;
      break;

    }
  }
  closedir(currentDirectory);

  //checks config file paths
  //if foundFlag is still 0

  char *filePaths = NULL;

  if(!foundFlag){

    filePaths = malloc(sizeof(*filePaths) * 1024);

    DIR *filePathsDIR = opendir(filePaths);

    FILE *config = fopen(configFilePath, "r");

    while(!foundFlag && fgets(filePaths, 1024, config)){

      while((!foundFlag) && ((d = readdir(filePathsDIR)) != NULL)){
	length = strlen(d->d_name);

	if(!memcmp(d->d_name, functionName, length - 3) &&
	   !strcmp(d->d_name + (length - 3), ".cr")){

	  foundFlag = 1;
	}
      }

    }

    fclose(config);

  }

  if(foundFlag){
    out = malloc(sizeof(d->d_name) * (strlen(functionName) + strlen(filePaths) + 2));
    strcpy(out, d->d_name);
    strcat(out, "/");
    strcat(out, functionName);

  }

  free(filePaths);

  return out;
}


matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, error_return *error){
  int argNo = numberOfArgs(functionArgs);

  FILE *userFunction = fopen(functionPath, "r");

  char title[1024];
  fgets(title, 1024, userFunction);

  vari *functionVar = newVari();

  if(!memcmp(title, "function", 8)){

    int i = 9;
    char outBuffer[1024];

    for(; title[i] == '='; ++i){
      outValue[i] = title[i];
    }

    char *outName = removeSpaces(outBuffer);

    *error = setVariable(functionVar, outName, NULL, -1);

    char **functionArgNames = separateString(*(title+ 
  }
  matrix *out = NULL;

  return out;  
}
