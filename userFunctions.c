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
  char *functionPath = findFunctionPath(functionName, error);

  if(!functionPath){
    *error = -5;
    return NULL;
  }

  matrix *out = executeUserFunction(functionPath, functionArgs, var, error);

  free(functionPath);

  return out;  
}


//returns the path to the function
char *findFunctionPath(char *functionName, error_return *error){
  struct dirent *d;
  DIR *currentDirectory = opendir(".");

  uint8_t foundFlag = 0;

  int length = 0;

  char *fileDirectory = malloc(sizeof(*fileDirectory) * 1024);
  __MALLOC_CHECK(fileDirectory, *error);

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

  char *filePaths = NULL;

  //checks config file paths
  //if foundFlag is still 0
  if(!foundFlag){
    filePaths = malloc(sizeof(*filePaths) * 1024);
    __MALLOC_CHECK(filePaths, *error);

    DIR *filePathsDIR = opendir(filePaths);

    FILE *config = fopen(configFilePath, "r");
    //read directories from config
    while(!foundFlag && fgets(filePaths, 1024, config)){
      //check each directory for file
      while((!foundFlag) && ((d = readdir(filePathsDIR)) != NULL)){
	length = strlen(d->d_name);
	//file ends in .cr, and its name matches
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


  //if the file was found
  if(foundFlag){
    int filePathLength = 0;

    if(filePaths){
      filePathLength = strlen(filePaths);
    }

    out = malloc(sizeof(out) * (strlen(functionName) + filePathLength + 5));
    __MALLOC_CHECK(out, *error);

    strcpy(out, fileDirectory);

  } else{
    *error = -8;
  }

  free(filePaths);
  free(fileDirectory);
  
  return out;
}


matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, error_return *error){
  int argNo = numberOfArgs(functionArgs);

  FILE *userFunction = fopen(functionPath, "r");

  char title[1024];
  memset(title, 0, 1024);
  fgets(title, 1024, userFunction);
  fclose(userFunction);

  vari *functionVar = newVari();
  matrix *out = NULL;
  
  if(!memcmp(title, "function", 8)){
    int i = 9;
    char outBuffer[1024];

    for(int j = 0; title[i] != '='; ++i, ++j){
      outBuffer[j] = title[i];
      outBuffer[j + 1] = '\0';
    }

    char *outName = removeSpaces(outBuffer);

    for(;title[i] != '('; ++i);
    int j = i--;

    for(; title[j] != ')'; ++j);
    title[j + 1] = '\0';

    char **functionArgNames = separateString(title, "()", ",", &i, error);
    int functionArgNo = numberOfArgs(functionArgNames);

    if(functionArgNo == argNo){
      char *inputName = removeSpaces(functionArgNames[0]);
      *error = sya(functionArgs[0], var);

      int variableIndex = -1;
      *error = setVariable(functionVar, inputName, copyMatrix(var->ans, error), &variableIndex);
      
      for(int j = 1; j < functionArgNo; ++j){
	inputName = removeSpaces(functionArgNames[j]);
	*error = sya(functionArgs[j], var);
	if(*error) break;

	variableIndex = -2;
	*error = setVariable(functionVar, inputName, copyMatrix(var->ans, error), &variableIndex);
	if(*error) break;

      }



      *error = runFile(&functionPath, functionVar, 1);

      int outVariable = varcheck(functionVar, outName);

      if(outVariable < 0){
	*error = -12;
      }

      out = copyMatrix(functionVar->value[outVariable], error);

    } else{
      *error = -2;

    }

    freeDoubleArray(functionArgNames);

  } else{
    *error = -13;

  }


  freeVari(functionVar);

  return out;  
}
