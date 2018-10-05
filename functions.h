#ifndef FUNCTIONS
#define FUNCTIONS

ele factorial(ele a);

matrix *eye(matrix *a, err_ret *error);
matrix *getSize(matrix *a, err_ret *error);
matrix *magnitude(matrix *a, err_ret *error);
matrix *numel(matrix *a, err_ret *error);

matrix *reference(matrix *a, matrix *b, err_ret *error);
matrix *assign(matrix *a, matrix *b, vari *var, err_ret *error);

matrix *divideMatrix(matrix *a, matrix *b, err_ret *error);
matrix *multiplyMatrix(matrix *a, matrix *b, err_ret *error);
matrix *exponentMatrix(matrix *a, matrix *b, err_ret *error);
matrix *transposeMatrix(matrix *a, err_ret *error);

matrix *min(matrix *m, err_ret *error);
matrix *max(matrix *m, err_ret *error);
matrix *sum(matrix *m, err_ret *error);
matrix *avg(matrix *m, err_ret *error);


ele oneArg(ele a, int o, err_ret *error);
ele twoArg(ele a, ele b, int o, err_ret *error);

#endif //FUNCTIONS
