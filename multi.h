#ifndef MULTI
#define MULTI

int numberOfArgs(char **input);

matrix *min(matrix *m, error_return *error);
matrix *max(matrix *m, error_return *error);
matrix *avg(matrix *m, error_return *error);

matrix *deri(char **input, vari *var, error_return *error);
matrix *inte(char **input, vari *var, error_return *error);
matrix *solve(char **input, vari *var, error_return *error);

matrix *zeros(char **input, vari *var, error_return *error);
matrix *ones(char **input, vari *var, error_return *error);
matrix *randMatrix(char **input, vari *var, error_return *error);

matrix *linspace(char **input, vari *var, error_return *error);
matrix *extractValue(char *buffer, char **input, int k, vari *var, error_return *error);

error_return printLine(char **input, vari *var);
char **separateString(char *input, char limits[2], char delimiter, int *start, error_return *error);

#endif //MULTI
