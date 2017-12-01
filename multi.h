#ifndef MULTI
#define MULTI

int numberOfArgs(char **input);
double min(char **input, vari *var, int *error);
double max(char **input, vari *var, int *error);
double avg(char **input, vari *var, int *error);
double deri(char **input, vari *var, int *error);
double inte(char **input, vari *var, int *error);
double solve(char **input, vari *var, int *error);
int printLine(char **input, vari *var, int *error);
char **separateString(char *input, char delimiter, int *start, int *error);

#endif //MULTI
