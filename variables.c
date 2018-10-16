#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "variables.h"

vari *init_var(int size) {
	vari *var = malloc(sizeof(*var));
	if(!var)
		return NULL;

	var->value = calloc(size, sizeof(*var->value));
	if(!var->value)
		return NULL;

	var->name = calloc(size, sizeof(*var->name));
	if(!var->name)
		return NULL;
	
	var->assign = NULL;

	var->ans = init_scalar(0);
	if(!var->ans)
		return NULL;

	var->count = -1;
	var->f_assign = 0;
	return var;
}


vari *cpy_var(vari *var, err_ret *error){
	vari *out = calloc(1, sizeof(*out));

	out->ans = calloc(1, sizeof(*out->ans));

	out->assign = NULL;
	out->f_assign = var->f_assign;

	out->count = var->count;

	if(var->count > -1){
		int i = 0;
		for(; i < var->count; ++i) {
			out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
			strcpy(out->name[i], var->name[i]);

			out->value[i] = cpy_mat(var->value[i], error);
			out->value[i]->var = 1;
		}


		if((var->value[i] != NULL) && (var->value[i]->size != NULL)) {
			out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
			strcpy(out->name[i], var->name[i]);

			out->value[i] = cpy_mat(var->value[i], error);
			out->value[i]->var = 1;
		}
		++i;
		out->name[i] = NULL;
		out->value[i] = NULL;
	}

	return out;
}


int find_var(vari *list, char *input) {
	if(list->count < 0)
		return -1;

	for(int i = 0; i<=list->count; ++i) {
		if(!strcmp(input, list->name[i]))
			return i;
	}

	return -2;
}


int set_var(vari *var, char *name, matrix *a, err_ret *error){
	//err_ret set_var(vari *var, char *name, matrix *a, int *check){
	//check is from the output of find_var

	int index = find_var(var, name);

	switch(index) {
	case -1: //new variable, struct is empty
		index = 0;
		var->count = 0;
		break;

	case -2: //new variable, struct is not empty
		index = ++var->count;
		break;

	default: //variable exists already
		free(var->name[index]);
		var->value[index]->var = 0;
		free_mat(var->value[index]);
		break;
	}

	var->name[index] = malloc(sizeof(*var->name[index]) * (strlen(name) + 1));
	__MALLOC_CHECK(var->name[index], *error);

	strcpy(var->name[index], name);
	var->value[index] = a;

	var->value[index]->var = 1;

	return index;
}


void free_var(vari *var){

	for(int i = 0; var->name[i]; ++i)
		free(var->name[i]);

	for(int i = 0; var->value[i]; ++i) {
		var->value[i]->var = 0;
		free_mat(var->value[i]);
	}

	free_mat(var->ans);
	if(var->assign)
		free_mat(var->assign);

	free(var->value);
	free(var->name);

	free(var);
}
