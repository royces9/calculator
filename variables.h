#ifndef VARIABLE
#define VARIABLE

typedef struct { //variable storage
	//pointer to matrix index to assign to, if assignment
	matrix *assignIndex;

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
	int8_t assignFlag;
} vari;

vari *newVari(void);
vari *copyVari(vari *var, error_return *error);

int varcheck(vari *list, char *input);

error_return setVariable(vari *var, char *name, matrix *a, int *check);
void freeVari(vari *var);


#endif //VARIABLE
