#ifndef FUNCTIONS
#define FUNCTIONS

ele factorial(ele a);

matrix *eye(matrix *a, err_ret *error);
matrix *get_size(matrix *a, err_ret *error);
matrix *magnitude(matrix *a, err_ret *error);
matrix *numel(matrix *a);

matrix *reference(matrix *a, matrix *b, err_ret *error);
matrix *assign(matrix *a, matrix *b, vari *var, err_ret *error);

//matrix *div_mat(matrix *a, matrix *b, err_ret *error);
matrix *mult_mat(matrix *a, matrix *b, err_ret *error);
matrix *exp_mat(matrix *a, matrix *b, err_ret *error);
matrix *t_mat(matrix *a, err_ret *error);

matrix *min(matrix *m, err_ret *error);
matrix *max(matrix *m, err_ret *error);
matrix *sum(matrix *m, err_ret *error);
matrix *avg(matrix *m, err_ret *error);


ele one_arg(ele a, int o, err_ret *error);
ele two_arg(ele a, ele b, int o, err_ret *error);

#endif //FUNCTIONS
