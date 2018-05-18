#ifndef MULTI
#define MULTI

int numberOfArgs(char **input);

matrix *min(matrix *m, int8_t *error);
matrix *max(matrix *m, int8_t *error);
matrix *avg(matrix *m, int8_t *error);

matrix *deri(char **input, vari *var, int8_t *error);
matrix *inte(char **input, vari *var, int8_t *error);
matrix *solve(char **input, vari *var, int8_t *error);

matrix *zeros(char **input, vari *var, int8_t *error);
matrix *ones(char **input, vari *var, int8_t *error);
matrix *randMatrix(char **input, vari *var, int8_t *error);

matrix *linspace(char **input, vari *var, int8_t *error);
matrix *extractValue(char *buffer, char **input, vari *var, int8_t *error);

int8_t printLine(char **input, vari *var);
char **separateString(char *input, char limits[2], char delimiter, int *start, int8_t *error);

#endif //MULTI
