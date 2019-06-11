#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "operator.h"

err_ret init_var(int size, struct vari **out) {
	*out = malloc(sizeof(**out));
	if(!(*out))
		return -6;

	out[0]->value = calloc(size, sizeof(*out[0]->value));
	if(!out[0]->value) {
		free(*out);
		return -6;
	}

	out[0]->name = calloc(size, sizeof(*out[0]->name));
	if(!out[0]->name) {
		free(out[0]->value);
		free(*out);
		return -6;
	}
	
	out[0]->assign = NULL;

	err_ret err = init_scalar(0, &out[0]->ans);
	if(err) {
		free(out[0]->name);
		free(out[0]->value);
		free(*out);
		return err;
	}

	out[0]->count = -1;
	out[0]->f_assign = 0;
	out[0]->size = size;
	return err;
}



err_ret cpy_var(struct vari *var, struct vari **out){
	err_ret err = init_var(var->size, out);
	if(err)
		return err;

	out[0]->f_assign = var->f_assign;

	out[0]->count = var->count;

	if(var->count > -1){
		int i = 0;
		for(; i < var->count; ++i) {
			out[0]->name[i] = malloc((strlen(var->name[i]) + 1) * sizeof(*var->name[i]));
			if(!out[0]->name[i]) {
				err = -6;
				goto err_ret;
			}

			strcpy(out[0]->name[i], var->name[i]);

			err = cpy_mat(var->value[i], &out[0]->value[i]);
			if(err)
				goto err_ret;

			out[0]->value[i]->var = 1;
		}


		if((var->value[i] != NULL) && (var->value[i]->size != NULL)) {
			out[0]->name[i] = malloc((strlen(var->name[i]) + 1) * sizeof(*var->name[i]));
			if(!out[0]->name[i]) {
				err = -6;
				goto err_ret;
			}

			strcpy(out[0]->name[i], var->name[i]);

			err = cpy_mat(var->value[i], &out[0]->value[i]);
			if(err)
				goto err_ret;

			out[0]->value[i]->var = 1;
		}
		++i;
		out[0]->name[i] = NULL;
		out[0]->value[i] = NULL;
	}

	return err;

 err_ret:
	free(out[0]->ans);
	free(*out);

	return err;
}


int find_var(struct vari *list, char *input) {
	if(list->count < 0)
		return -1;

	int i = search_str(input, (char const *const *const) list->name);
	if(i > list->count)
		i = -2;

	return i;
}


int set_var(struct vari *var, char *name, struct matrix *a, err_ret *error) {
	int index = find_var(var, name);

	switch(index) {
	case -1: //new variable, struct is empty
		index = 0;
		var->count = 0;
		break;

	case -2: //new variable, struct is not empty
		++var->count;
		index = var->count;
		break;

	default: //variable exists already
		free(var->name[index]);
		var->value[index]->var = 0;
		free_mat(var->value[index]);
		break;
	}

	var->name[index] = malloc(sizeof(*var->name[index]) * (strlen(name) + 1));
	if(!var->name[index]) {
		*error = -6;
		return 0;
	}

	strcpy(var->name[index], name);
	var->value[index] = a;

	var->value[index]->var = 1;

	return index;
}


void free_var(struct vari *var){

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
