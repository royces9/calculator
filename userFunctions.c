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
matrix *find_user_fun(char *name, char **args, vari *var, err_ret *error) {
	matrix *out = NULL;
	//get the path to the file
	char *path = find_path(name, error);

	if(!path) {
		*error = -5;
	} else {
		out = exec_fun(path, args, var, error);
	}

	free(path);

	return out;  
}


//checks a config file with a list of paths to check if functionName exists
char *chk_conf(char *name, char *configPath, err_ret *error) {
	char filePaths[1024];

	char *out = NULL;
	char *file_dir = NULL;

	FILE *config = fopen(configPath, "r");
	if( !config ) {
		*error = -8;
	} else {
		//read directories from config
		while(fgets(filePaths, 1024, config)) {
			file_dir = search_dir(name, filePaths, error);

			if(file_dir) {
				out = malloc(sizeof(*out) * (strlen(filePaths) + strlen(file_dir) + 1));
				__MALLOC_CHECK(out, *error);
				strcpy(out, filePaths);
				strcat(out, file_dir);
				free(file_dir);
				break;
			}
		}
	}

	return out;
}


//returns the path to the function
char *find_path(char *name, err_ret *error) {
	char *file_dir = search_dir(name, ".", error);

	//checks config file paths
	//if fileDirectory is still NULL
	if(!file_dir) {
		//assume that the user is NOT going to use sudo
		char *home = getenv("HOME");
		char *configPath = "/.config/calc.conf";
		char *config = malloc(sizeof(*config) * (strlen(home) + strlen(configPath) + 1));

		strcpy(config, home);
		strcat(config, configPath);

		file_dir = chk_conf(name, config, error);

		if(!file_dir)
			*error = -8;

		free(config);
	}
  
	return file_dir;
}


matrix *exec_fun(char *functionPath, char **functionArgs, vari *var, err_ret *error) {
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

	matrix *out = NULL;
	//confirm that the function is the first word in the file
	if(!strncmp(title, "function", 8)) {
		vari *fun_var = init_var();
		char out_buff[1024];

		int i = 9;
		//finds the name of the output variable
		//starts from the first char after function
		//to where the equal is
		for(int j = 0; title[i] != '='; ++i, ++j) {
			out_buff[j] = title[i];
			out_buff[j + 1] = '\0';
		}

		//remove spaces from the name
		char *outName = removeSpaces(out_buff);

		//increment title to where input arguments are
		//first find left end paren
		for(;title[i] != '('; ++i);
		int j = i--;

		//then find right end paren
		for(; title[j] != ')'; ++j);
		title[j + 1] = '\0';

		//separate the string, to know what the variable names are
		char **arg_names = sep_str(title, "()", ",", (uint16_t *) &i, error);

		//count the number of arguments required
		int functionArgNo = numberOfArgs(arg_names);

		//check that the given arguments match with the
		//require number of arguments
		if(functionArgNo == argNo) {
			//remove spaces from the names
			char *inputName = removeSpaces(arg_names[0]);

			//calcuate value of first input argument
			//gets stored in var->ans
			//use var instead of functionVar because the input
			//arguments might have variable in them
			*error = sya(functionArgs[0], var);
			if(*error)
				goto ret_out;

			//set the first variable to the corresponding name and value
			set_var(fun_var, inputName, cpy_mat(var->ans, error), error);
			if(*error)
				goto ret_out;
      
			//do the rest for the other variables, if necessary
			for(int j = 1; j < functionArgNo; ++j) {
				inputName = removeSpaces(arg_names[j]);
				*error = sya(functionArgs[j], var);
				if(*error)
					goto ret_out;

				set_var(fun_var, inputName, cpy_mat(var->ans, error), error);

				if(*error)
					goto ret_out;
			}

			//make sure no errors in for loop
			if(!(*error)) {
				//run the file
				//offset by one line
				*error = runFile(&functionPath, fun_var, 1);
				if(*error)
					goto ret_out;

				//check that the out variable exists
				int out_var = find_var(fun_var, outName);

				if(out_var < 0)
					*error = -12;
				
				out = cpy_mat(fun_var->value[out_var], error);
			}

		} else {
			*error = -2;

		}

	ret_out:
		freeDoubleArray(arg_names);
		free_var(fun_var);

	} else {
		*error = -8;

	}

	return out;  
}


uint8_t chk_name(char *name, char *dir) {
	int len = strlen(name);	
	return !strncmp(name, dir, len) && !strncmp(dir + len, ".cr", 4);
}


char *search_dir(char *name, char *dir_name, err_ret *error) {
	char *out = NULL;
	DIR *dir = opendir(dir_name);

	if(!dir) {
		*error = -8;
	} else {
		struct dirent *d;

		while( (d = readdir(dir)) ) {
			uint16_t len = strlen(d->d_name);		

			//checks that function name is the same, and ends in ".cr"
			if(chk_name(name, d->d_name)) {
				out = malloc(sizeof(*out) * (len + 1));
				__MALLOC_CHECK(out, *error);
				strcpy(out, d->d_name);
				break;
			}
		}
	}
	closedir(dir);
	
	return out;
}
