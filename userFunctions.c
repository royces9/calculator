#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"
#include "variables.h"

#include "multi.h"
#include "sya.h"
#include "operator.h"
#include "file.h"
#include "userFunctions.h"

char *configFilePath = "~/.calc.cfg";

//find and execute a user made function
matrix *findUserFunction(char *functionName, char **functionArgs, vari *var, err_ret *error) {
	//get the path to the file
	matrix *out = NULL;
	char *functionPath = findFunctionPath(functionName, error);

	if(!functionPath) {
		*error = -5;

	} else {
		out = executeUserFunction(functionPath, functionArgs, var, error);

	}
	free(functionPath);

	return out;  
}


//checks a config file with a list of paths to check if functionName exists
char *checkConfig(char *functionName, char *configPath, err_ret *error) {
	struct dirent *d;
	char filePaths[1024];
	uint8_t foundFlag = 0;

	char *fileDirectory = NULL;

	FILE *config = fopen(configPath, "r");
	if( !config ) {
		*error = 8;

	} else {
		//read directories from config
		while(!foundFlag && fgets(filePaths, 1024, config)) {
			//for each path, search directory
			DIR *filePathsDirectory = opendir(filePaths);
			if( !filePathsDirectory ) {
				*error = -8;
				break;
			}

			//check each directory for file
			while((!foundFlag) && (d = readdir(filePathsDirectory))) {
				uint16_t length = strlen(d->d_name);

				//file ends in .cr, and its name matches
				if(checkFunctionName(functionName, d->d_name)) {
					fileDirectory = malloc(sizeof(*fileDirectory) * (length + 1));
					strcpy(fileDirectory, d->d_name);
					foundFlag = 1;
				}
			}
			closedir(filePathsDirectory);
		}
	}
	fclose(config);

	return fileDirectory;
}


//returns the path to the function
char *findFunctionPath(char *functionName, err_ret *error) {
	struct dirent *d;
	DIR *currentDirectory = opendir(".");

	uint8_t foundFlag = 0;

	char *fileDirectory = malloc(sizeof(*fileDirectory) * 1024);
	__MALLOC_CHECK(fileDirectory, *error);

	char *out = NULL;

	//checks the current directory
	while((d = readdir(currentDirectory)) != NULL) {
		//checks that function name is the same, and ends in ".cr"
		if(checkFunctionName(functionName, d->d_name)) {
			strcpy(fileDirectory, d->d_name);
			foundFlag = 1;
			break;

		}
	}

	closedir(currentDirectory);

	char *filePaths = NULL;

	//checks config file paths
	//if foundFlag is still 0
	if(!foundFlag) {

		char *configPath = "";
		fileDirectory = checkConfig(functionName, configPath, error);
		if(out)
			foundFlag = 1;
	}

	//if the file was found
	if(foundFlag) {
		int filePathLength = 0;

		if(filePaths) {
			filePathLength = strlen(filePaths);
		}

		out = malloc(sizeof(*out) * (strlen(functionName) + filePathLength + 5));
		__MALLOC_CHECK(out, *error);

		strcpy(out, fileDirectory);

	} else {
		*error = -8;
	}

	free(filePaths);
	free(fileDirectory);
  
	return out;
}


matrix *executeUserFunction(char *functionPath, char **functionArgs, vari *var, err_ret *error) {
	int argNo = numberOfArgs(functionArgs);

	FILE *userFunction = fopen(functionPath, "r");

	//get the header for the function
	//right now it's hardcoded to get the first line
	char title[1024];
	memset(title, 0, 1024);
	fgets(title, 1024, userFunction);
	fclose(userFunction);

	//variable struct for the function
	//essentially a new scope
	vari *functionVar = init_var();
	matrix *out = NULL;
  
	//confirm that the function is the first word in the file
	if(!memcmp(title, "function", 8)) {
		char outBuffer[1024];

		int i = 9;
		//finds the name of the output variable
		//starts from the first char after function
		//to where the equal is
		for(int j = 0; title[i] != '='; ++i, ++j) {
			outBuffer[j] = title[i];
			outBuffer[j + 1] = '\0';
		}

		//remove spaces from the name
		char *outName = removeSpaces(outBuffer);

		//increment title to where input arguments are
		//first find left end paren
		for(;title[i] != '('; ++i);
		int j = i--;

		//then find right end paren
		for(; title[j] != ')'; ++j);
		title[j + 1] = '\0';

		//separate the string, to know what the variable names are
		char **functionArgNames = sep_str(title, "()", ",", (uint16_t *) &i, error);

		//count the number of arguments required
		int functionArgNo = numberOfArgs(functionArgNames);

		//check that the given arguments match with the
		//require number of arguments
		if(functionArgNo == argNo) {
			//remove spaces from the names
			char *inputName = removeSpaces(functionArgNames[0]);

			//calcuate value of first input argument
			//gets stored in var->ans
			//use var instead of functionVar because the input
			//arguments might have variable in them
			*error = sya(functionArgs[0], var);
			if(*error) {
				freeDoubleArray(functionArgNames);
				free_var(functionVar);
				return NULL;
			}

			//set the first variable to the corresponding name and value
			set_var(functionVar, inputName, cpy_mat(var->ans, error), error);
			if(*error) {
				freeDoubleArray(functionArgNames);
				free_var(functionVar);
				return NULL;
			}
      
			//do the rest for the other variables, if necessary
			for(int j = 1; j < functionArgNo; ++j) {
				inputName = removeSpaces(functionArgNames[j]);
				*error = sya(functionArgs[j], var);
				if(*error) break;

				set_var(functionVar, inputName, cpy_mat(var->ans, error), error);

				if(*error) break;

			}

			//make sure no errors in for loop
			if(!(*error)) {
				//run the file
				//offset by one line
				*error = runFile(&functionPath, functionVar, 1);
				if(*error) {
					freeDoubleArray(functionArgNames);
					free_var(functionVar);
					return NULL;
				}

				//check that the out variable exists
				int outVariable = find_var(functionVar, outName);

				if(outVariable < 0)
					*error = -12;
				
				out = cpy_mat(functionVar->value[outVariable], error);
			}

		} else {
			*error = -2;

		}

		freeDoubleArray(functionArgNames);

	} else {
		*error = -13;

	}

	free_var(functionVar);

	return out;  
}


uint8_t checkFunctionName(char *functionName, char *directory) {
	uint8_t out = 1;
	uint16_t function_length = strlen(functionName);

	char *tempStr = malloc(sizeof(*tempStr) * function_length + 4);
	strcpy(tempStr, functionName);
	strcat(tempStr, ".cr");

	if(strcmp(tempStr, directory))
		out = 0;

	free(tempStr);

	return out;

}