#ifndef FUNCTIONS
#define FUNCTIONS

element factorial(element a);

matrix *eye(matrix *a, error_return *error);
matrix *getSize(matrix *a, error_return *error);

matrix *assign(matrix **a, matrix *b, matrix *c, error_return *error);
matrix *divideMatrix(matrix *a, matrix *b, error_return *error);
matrix *multiplyMatrix(matrix *a, matrix *b, error_return *error);
matrix *exponentMatrix(matrix *a, matrix *b, error_return *error);
matrix *transposeMatrix(matrix *a, error_return *error);

element oneArg(element a, int o, error_return *error);
element twoArg(element a, element b, int o, error_return *error);

#endif //FUNCTIONS
