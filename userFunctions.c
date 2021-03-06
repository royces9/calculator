#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "types.h"

#include "multi.h"
#include "sya.h"
#include "operator.h"
#include "file.h"
#include "userFunctions.h"

//find and execute a user made function
err_ret find_user_fun(char *name, char **args, struct vari *var, struct matrix **out) {
	//get the path to the file
	char *path = NULL;
	err_ret err = find_path(name, &path);
	if(!err)
		err = exec_fun(path, args, var, out);

	free(path);

	return err;  
}


//checks a config file with a list of paths to check if functionName exists
err_ret chk_conf(char *name, char *cfg, char **out) {

	err_ret err = 0;
	FILE *f_cfg = fopen(cfg, "r");
	if( !f_cfg )
		return e_file_dne;

	char *paths = malloc(BUFF_SIZE * sizeof(*paths));
	if(!paths)
		return e_malloc;

	char *file_dir = NULL;

	//read directories from config
	while(fgets(paths, BUFF_SIZE, f_cfg)) {
		
		err = search_dir(name, paths, &file_dir);

		if(!err) {
			*out = malloc((strlen(paths) + strlen(file_dir) + 1) * sizeof(*out));
			if(!(*out))
				return e_malloc;

			strcpy(*out, paths);
			strcat(*out, file_dir);

			free(file_dir);
			break;
		}
	}

	free(paths);

	return err;
}


//returns the path to the function
err_ret find_path(char *name, char **path) {
	err_ret err = search_dir(name, ".", path);

	//checks config file paths
	//if fileDirectory is still NULL
	if(!err)
		return err;

	//assume that the user is NOT going to use sudo
	char *home = getenv("HOME");
	char *configPath = "/.config/calc.conf";

	int conf_len = strlen(home) + strlen(configPath) + 1;
	char *config = malloc(conf_len * sizeof(*config));
	if(!config) {
		*path = NULL;
		return e_malloc;
	}

	strcpy(config, home);
	strcat(config, configPath);

	err = chk_conf(name, config, path);

	free(config);
  
	return err;
}


err_ret exec_fun(char *path, char **args, struct vari *var, struct matrix **out) {
	int argNo = numberOfArgs(args);
	err_ret err = 0;

	FILE *userFunction = fopen(path, "r");
	if(!userFunction)
		return e_file_dne;


	//get the header for the function
	//right now it's hardcoded to get the first line
	char *title = calloc(BUFF_SIZE, sizeof(*title));
	if(!title)
		return e_malloc;
	
	fgets(title, BUFF_SIZE, userFunction);
	fclose(userFunction);

	//confirm that the function is the first word in the file
	if(strncmp(title, "function", 8))
		return e_file_dne;

	char *out_buff = malloc(BUFF_SIZE * sizeof(*out_buff));
	if(!out_buff)
		return e_malloc;

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
	int j = i;
	--i;
	
	//then find right end paren
	for(; title[j] != ')'; ++j);
	title[j + 1] = '\0';

	//separate the string, to know what the variable names are
	char **arg_names = sep_str(title, "()", ",", &i, &err);
	free(title);

	//count the number of arguments required
	int functionArgNo = numberOfArgs(arg_names);

	//check that the given arguments match with the
	//require number of arguments
	struct vari *fun_var = NULL;

	if(functionArgNo != argNo) {
		err = e_func_args;
		goto ret_out;
	}

	//variable struct for the function
	//essentially a new scope

	err = init_var(256, &fun_var);
	if(err)
		goto ret_out;

	for(int j = 0; j < functionArgNo; ++j) {
		char *inputName = removeSpaces(arg_names[j]);

		//calcuate value of first input argument
		//gets stored in var->ans
		//use var instead of functionVar because the input
		//arguments might have variable in them
		err = sya(args[j], var);
		if(err)
			goto ret_out;

		struct matrix *tmp_mat = NULL;
		err = cpy_mat(var->ans, &tmp_mat);
		if(err)
			goto ret_out;

		set_var(fun_var, inputName, tmp_mat, &err);
		if(err)
			goto ret_out;
	}

	//run the file
	//offset by one line
	err = runFile(&path, fun_var, 1);
	if(err)
		goto ret_out;

	//check that the out variable exists
	int out_var = find_var(fun_var, outName);
   	if(out_var < 0) {
		err = e_no_output;
	} else {
		err = cpy_mat(fun_var->value[out_var], out);
	}

 ret_out:
	free(out_buff);
	free_var(fun_var);

	freeDoubleArray(arg_names);

	return err;  
}


uint8_t chk_name(char *name, char *dir) {
	int len = strlen(name);	
	return !strncmp(name, dir, len) && !strncmp(dir + len, ".cr", 4);
}


err_ret search_dir(char *name, char *dir_name, char **out) {
	DIR *dir = opendir(dir_name);
	err_ret err = e_file_dne;
	if(!dir)
		return err;

	struct dirent *d;

	while( (d = readdir(dir)) ) {
		//checks that function name is the same, and ends in ".cr"
		if(chk_name(name, d->d_name)) {
			int len = strlen(d->d_name);
			*out = malloc((len + 1) * sizeof(*out));
			if(!(*out))
				return e_malloc;

			strcpy(*out, d->d_name);
			err = 0;
			break;
		}
	}

	closedir(dir);
	
	return err;
}
