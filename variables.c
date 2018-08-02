#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "variables.h"

vari *newVari(void) {
	vari *var = calloc(1, sizeof(*var));
	var->ans = initScalar(0, NULL);
	var->count = -1;
	return var;
}


vari *copyVari(vari *var, error_return *error){
	vari *out = malloc(sizeof(*out));

	out->ans = calloc(1, sizeof(*out->ans));

	out->assignIndex = NULL;
	out->assignFlag = var->assignFlag;

	out->count = var->count;


	if(var->count > -1){
		int i = 0;
		for(; i < var->count; ++i){
			out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
			strcpy(out->name[i], var->name[i]);

			out->value[i] = copyMatrix(var->value[i], error);
			out->value[i]->variable = 1;
		}


		if((var->value[i] != NULL) && (var->value[i]->size != NULL)){
			out->name[i] = malloc(sizeof(*var->name[i]) * (strlen(var->name[i]) + 1));
			strcpy(out->name[i], var->name[i]);

			out->value[i] = copyMatrix(var->value[i], error);
			out->value[i]->variable = 1;
		}
		++i;
		out->name[i] = NULL;
		out->value[i] = NULL;
	} else{
		memset(out->name, '\0', sizeof(out->name));
		memset(out->value, 0, sizeof(out->value));
	}

	return out;
}


int findVariable(vari *list, char *input) {
	if(list->count < 0) {
		return -1;
	}

	for(int i = 0; i<=list->count; ++i) {
		if(!strcmp(input, list->name[i]))
			return i;
	}

	return -2;
}


int setVariable(vari *var, char *name, matrix *a, error_return *error){
	//error_return setVariable(vari *var, char *name, matrix *a, int *check){
	//check is from the output of findVariable

	int index = findVariable(var, name);

	switch(index){
	case -1: //new variable, struct is empty
		index = 0;
		var->count = 0;
		break;

	case -2: //new variable, struct is not empty
		index = ++var->count;
		break;

	default: //variable exists already
		free(var->name[index]);
		var->value[index]->variable = 0;
		freeMatrix(var->value[index]);
		break;
	}

	var->name[index] = malloc(sizeof(*var->name[index]) * (strlen(name) + 1));
	__MALLOC_CHECK(var->name[index], *error);

	strcpy(var->name[index], name);
	var->value[index] = a;

	var->value[index]->variable = 1;

	return index;
}


void freeVari(vari *var){

	for(int i = 0; var->name[i]; ++i){
		free(var->name[i]);
	}

	for(int i = 0; var->value[i]; ++i){
		var->value[i]->variable = 0;
		freeMatrix(var->value[i]);
	}

	freeMatrix(var->ans);

	if(var->assignIndex != NULL){
		freeMatrix(var->assignIndex);
	}

	free(var);
}
