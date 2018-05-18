#ifndef FUNCTIONS
#define FUNCTIONS

element factorial(element a);

matrix *eye(matrix *a, int8_t *error);
matrix *getSize(matrix *a, int8_t *error);

matrix *divideMatrix(matrix *a, matrix *b, int8_t *error);
matrix *multiplyMatrix(matrix *a, matrix *b, int8_t *error);
matrix *exponentMatrix(matrix *a, matrix *b, int8_t *error);
matrix *transposeMatrix(matrix *a, int8_t *error);

element oneArg(element a, int o, int8_t *error);
element twoArg(element a, element b, int o, int8_t *error);

#endif //FUNCTIONS
