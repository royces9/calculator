#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "operator.h"

vari *init_var(int size) {
	vari *var = malloc(sizeof(*var));
	if(!var)
		return NULL;

	var->value = calloc(size, sizeof(*var->value));
	if(!var->value) {
		free(var);
		return NULL;
	}

	var->name = calloc(size, sizeof(*var->name));
	if(!var->name) {
		free(var->value);
		free(var);
		return NULL;
	}
	
	var->assign = NULL;

	var->ans = init_scalar(0);
	if(!var->ans) {
		free(var->name);
		free(var->value);
		free(var);
		return NULL;
	}

	var->count = -1;
	var->f_assign = 0;
	var->size = size;
	return var;
}


vari *cpy_var(vari *var){
	vari *out = init_var(var->size);
	if( !out )
		return NULL;

	out->f_assign = var->f_assign;

	out->count = var->count;

	if(var->count > -1){
		int i = 0;
		for(; i < var->count; ++i) {
			if( !(out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1))) )
				goto err_ret;

			strcpy(out->name[i], var->name[i]);

			if( !(out->value[i] = cpy_mat(var->value[i])) )
				goto err_ret;

			out->value[i]->var = 1;
		}


		if((var->value[i] != NULL) && (var->value[i]->size != NULL)) {
			if( !(out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1))) )
				goto err_ret;

			strcpy(out->name[i], var->name[i]);

			if( !(out->value[i] = cpy_mat(var->value[i])) )
				goto err_ret;

			out->value[i]->var = 1;
		}
		++i;
		out->name[i] = NULL;
		out->value[i] = NULL;
	}

	return out;

 err_ret:
	free(out->ans);
	free(out);
	return NULL;
}


int find_var(vari *list, char *input) {
	if(list->count < 0)
		return -1;

	int i = search_str(input, (char const *const *const) list->name);
	if(i == (list->count + 1))
		i = -2;

	return i;
}


int set_var(vari *var, char *name, matrix *a, err_ret *er) {
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

	if( !(var->name[index] = malloc(sizeof(*var->name[index]) * (strlen(name) + 1))) )
		return NULL;

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
