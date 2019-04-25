#ifndef FUNCTIONS
#define FUNCTIONS



matrix *eye(matrix *a, err_ret *error);
matrix *get_size(matrix *a, err_ret *error);
matrix *magnitude(matrix *a, err_ret *error);
matrix *numel(matrix *a);

matrix *reference(matrix *a, matrix *b, err_ret *error);
matrix *assign(matrix *a, matrix *b, vari *var, err_ret *error);

matrix *div_mat(matrix *a, matrix *b, err_ret *error);
matrix *mult_mat(matrix *a, matrix *b, err_ret *error);
matrix *exp_mat(matrix *a, matrix *b, err_ret *error);
matrix *t_mat(matrix *a, err_ret *error);

matrix *min(matrix *m, err_ret *error);
matrix *max(matrix *m, err_ret *error);
matrix *sum(matrix *m, err_ret *error);
matrix *avg(matrix *m, err_ret *error);


ele _sin(ele a, err_ret *e);
ele _cos(ele a, err_ret *e);
ele _tan(ele a, err_ret *e);
ele _log(ele a, err_ret *e);
ele _log10(ele a, err_ret *e);
ele _sqrt(ele a, err_ret *e);
ele _asin(ele a, err_ret *e);
ele _acos(ele a, err_ret *e);
ele _atan(ele a, err_ret *e);
ele _floor(ele a, err_ret *e);
ele _ceil(ele a, err_ret *e);
ele _round(ele a, err_ret *e);

ele factorial(ele a, err_ret *e);

ele one_arg(ele a, int o, err_ret *error);
ele two_arg(ele a, ele b, int o, err_ret *error);

#endif //FUNCTIONS
