#ifndef FUNCTIONS
#define FUNCTIONS

element factorial(element a);

matrix *eye(matrix *a, int *error);
matrix *getSize(matrix *a, int *error);

matrix *divideMatrix(matrix *a, matrix *b, int *error);
matrix *multiplyMatrix(matrix *a, matrix *b, int *error);
matrix *exponentMatrix(matrix *a, matrix *b, int *error);
matrix *transposeMatrix(matrix *a, int *error);

element oneArg(element a, int o, int *error);
element twoArg(element a, element b, int o, int *error);

#endif //FUNCTIONS
