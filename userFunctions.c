#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

#include "operator.h"
#include "sya.h"
#include "stack.h"
#include "file.h"
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

  error_return error = 0;

  struct dirent *d;
  DIR *currentDirectory = opendir(".");

  uint8_t foundFlag = 0;

  int length = 0;

  char *fileDirectory = malloc(sizeof(*fileDirectory) * 1024);
  __MALLOC_CHECK(fileDirectory, error);

  char *out = NULL;

  //checks the current directory
  while((d = readdir(currentDirectory)) != NULL){
    length = strlen(d->d_name);

    //checks that function name is the same, and ends in ".cr"
    if(!memcmp(d->d_name, functionName, length - 3) &&
       !strcmp(d->d_name + (length - 3), ".cr")){

      strcpy(fileDirectory, d->d_name);
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

	  strcpy(fileDirectory, d->d_name);
	  foundFlag = 1;
	}
      }

    }

    fclose(config);
    closedir(filePathsDIR);
  }

  if(foundFlag){
    int filePathLength = 0;

    if(filePaths){
      filePathLength = strlen(filePaths);
    }

    out = malloc(sizeof(out) * (strlen(functionName) + filePathLength + 5));
    strcpy(out, fileDirectory);

  }

  free(filePaths);
  free(fileDirectory);
  
  return out;
}


matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, error_return *error){
  int argNo = numberOfArgs(functionArgs);

  printf("%s\n", functionPath);

  FILE *userFunction = fopen(functionPath, "r");

  char title[1024];
  fgets(title, 1024, userFunction);

  vari *functionVar = newVari();
  matrix *out = NULL;
  
  if(!memcmp(title, "function", 8)){
    int i = 9;
    char outBuffer[1024];

    for(; title[i] == '='; ++i){
      outBuffer[i] = title[i];
    }

    char *outName = removeSpaces(outBuffer);

    int temp = -1;
    *error = setVariable(functionVar, outName, NULL, &temp);
    printf("%s\n", title);
    char **functionArgNames = separateString(title + 9, "()", ',', &i, error);

    int functionArgNo = numberOfArgs(functionArgNames);

    if(functionArgNo == argNo){
      for(int j = 0; j < functionArgNo; ++j){
	*error = sya(functionArgs[j], var);
	if(*error) break;

	temp = -2;
	*error = setVariable(functionVar, functionArgNames[j], var->ans, &temp);
	if(*error) break;

      }

    } else{
      *error = -2;

    }

    fclose(userFunction);
    freeDoubleArray(functionArgNames);

    *error = runFile(&functionPath, functionVar);
    out = copyMatrix(var->ans, error);

  } else{
    *error = -13;

  }


  return out;  
}
