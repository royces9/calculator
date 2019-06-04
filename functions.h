#ifndef FUNCTIONS
#define FUNCTIONS

struct matrix *eye(struct matrix *a, err_ret *error);
struct matrix *get_size(struct matrix *a, err_ret *error);
struct matrix *magnitude(struct matrix *a, err_ret *error);
struct matrix *numel(struct matrix *a, err_ret *error);

struct matrix *reference(struct matrix *a, struct matrix *b, err_ret *error);
struct matrix *assign(struct matrix *a, struct matrix *b, struct vari *var, err_ret *error);

struct matrix *div_mat(struct matrix *a, struct matrix *b, err_ret *error);
struct matrix *mult_mat(struct matrix *a, struct matrix *b, err_ret *error);
struct matrix *exp_mat(struct matrix *a, struct matrix *b, err_ret *error);
struct matrix *t_mat(struct matrix *a, err_ret *error);

struct matrix *min(struct matrix *m, err_ret *error);
struct matrix *max(struct matrix *m, err_ret *error);
struct matrix *sum(struct matrix *m, err_ret *error);
struct matrix *avg(struct matrix *m, err_ret *error);


ele _sin(ele a);
ele _cos(ele a);
ele _tan(ele a);
ele _log(ele a);
ele _log10(ele a);
ele _sqrt(ele a);
ele _asin(ele a);
ele _acos(ele a);
ele _atan(ele a);
ele _floor(ele a);
ele _ceil(ele a);
ele _round(ele a);

ele factorial(ele a);

ele one_arg(ele a, int o, err_ret *error);
ele two_arg(ele a, ele b, int o, err_ret *error);

#endif //FUNCTIONS
