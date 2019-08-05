#ifndef FUNCTIONS
#define FUNCTIONS

err_ret eye(struct matrix const *const a, struct matrix **out);
err_ret get_size(struct matrix const *const a, struct matrix **out);
err_ret magnitude(struct matrix const *const a, struct matrix **out);
err_ret numel(struct matrix const *const a, struct matrix **out);

err_ret reference(struct matrix const *const a, struct matrix const *const b, struct matrix **out);
err_ret assign(struct matrix *a, struct matrix *b, struct vari *var, struct matrix **out);

err_ret div_mat(struct matrix const *const a, struct matrix const *const b, struct matrix **out);
err_ret mult_mat(struct matrix const *const a, struct matrix const *const b, struct matrix **out);
err_ret exp_mat(struct matrix const *const a, struct matrix const *const b, struct matrix **out);
err_ret t_mat(struct matrix const *const a, struct matrix **out);

err_ret min(struct matrix const *const m, struct matrix **out);
err_ret max(struct matrix const *const m, struct matrix **out);
err_ret sum(struct matrix const *const m, struct matrix **out);
err_ret avg(struct matrix const *const m, struct matrix **out);

err_ret determinant(struct matrix const *const m, struct matrix **out);

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

#endif //FUNCTIONS
