#ifndef MULTI
#define MULTI

double vartypeset(vari *var, char input[]);
double deri(char **input, vari *var, int *error);
double inte(char **input, vari *var, int *error);
double solve(char **input, vari *var, int *error);
char **separateString(char *input, int *start);

#endif //MULTI
