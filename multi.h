#ifndef MULTI
#define MULTI

int numberOfArgs(char **input);
matrix *min(char **input, vari *var, int *error);
matrix *max(char **input, vari *var, int *error);
matrix *avg(char **input, vari *var, int *error);
matrix *deri(char **input, vari *var, int *error);
matrix *inte(char **input, vari *var, int *error);
matrix *solve(char **input, vari *var, int *error);
int printLine(char **input, vari *var, int *error);
char **separateString(char *input, char delimiter, int *start, int *error);

#endif //MULTI
