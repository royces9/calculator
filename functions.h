#ifndef FUNCTIONS
#define FUNCTIONS

element factorial(element a);

matrix *eye(matrix *a, int *error);
matrix *getSize(matrix *a, int *error);

matrix *matrixMultiply(matrix *a, matrix *b, int *error);

element oneArg(element a, int o, int *error);
element twoArg(element a, element b, int o, int *error);

#endif //FUNCTIONS
