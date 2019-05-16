#ifndef VARIABLE
#define VARIABLE

struct vari { //variable storage
	//pointer to matrix index to assign to, if assignment
	struct matrix *assign;

	//stores variable values
	struct matrix **value;

	//stores variable names
	char **name;

	//answer matrix
	struct matrix *ans;

	//index for the newest variable
	//-1 if empty
	int count;

	//maximum number of variables held
	int size;

	//flag if an assignment occured
	int8_t f_assign;
};

struct vari *init_var(int size);
struct vari *cpy_var(struct vari *var);

//find variable named input in list
int find_var(struct vari *list, char *input);

int set_var(struct vari *var, char *name, struct matrix *a, err_ret *error);
void free_var(struct vari *var);


#endif //VARIABLE
