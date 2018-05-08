#ifndef MULTI
#define MULTI

int numberOfArgs(char **input);
matrix *min(matrix *m, vari *var, int *error);
matrix *max(matrix *m, vari *var, int *error);
matrix *avg(matrix *m, vari *var, int *error);
matrix *deri(char **input, vari *var, int *error);
matrix *inte(char **input, vari *var, int *error);
matrix *solve(char **input, vari *var, int *error);
matrix *zeros(char **input, vari *var, int *error);
matrix *ones(char **input, vari *var, int *error);
int printLine(char **input, vari *var, int *error);
char **separateString(char *input, char limits[2], char delimiter, int *start, int *error);

#endif //MULTI
