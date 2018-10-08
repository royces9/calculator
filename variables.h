#ifndef VARIABLE
#define VARIABLE

typedef struct { //variable storage
	//pointer to matrix index to assign to, if assignment
	matrix *assign;

	//stores variable values
	matrix *value[256];

	//stores variable names
	char *name[256];

	//answer matrix
	matrix *ans;

	//index for the newest variable
	//-1 if empty
	int count;

	//flag if an assignment occured
	int8_t f_assign;
} vari;

vari *init_var(void);
vari *cpy_var(vari *var, err_ret *error);

//find variable named input in list
int find_var(vari *list, char *input);

int set_var(vari *var, char *name, matrix *a, err_ret *error);
void free_var(vari *var);


#endif //VARIABLE
