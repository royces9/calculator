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

//find variable named input in list
int findVariable(vari *list, char *input);

int setVariable(vari *var, char *name, matrix *a, error_return *error);
void freeVari(vari *var);


#endif //VARIABLE
