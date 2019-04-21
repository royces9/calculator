#ifndef OPERATOR
#define OPERATOR

int search_str(char *buffer, char const *const list[]);
err_ret ex_num(stack *num, vari *var, op_struct *ch);

matrix *mat_one(matrix *a, op_struct *ch, err_ret *error);
matrix *mat_two(matrix *a, matrix *b, op_struct *ch, err_ret *error);

err_ret find_fun(char *buffer, stack *num, stack *ch, vari *var, int8_t *tok, uint16_t *iter, char *input);
err_ret find_op(char *buffer, stack *num, stack *ch, vari *var, int8_t *tok);

char **sep_mat(char *input, uint16_t length, err_ret *error);
matrix *ext_mat(vari *var, uint16_t *iter, char *input, err_ret *error);

void help_print(void);

ele add(ele a, ele b);
ele subtract(ele a, ele b);
ele multiply(ele a, ele b);
ele divide(ele a, ele b);
ele exponent(ele a, ele b);
ele less(ele a, ele b);
ele greater(ele a, ele b);
ele less_equal(ele a, ele b);
ele greater_equal(ele a, ele b);
ele not_equal(ele a, ele b);
ele equal(ele a, ele b);
ele and(ele a, ele b);
ele or(ele a, ele b);
ele modulo(ele a, ele b);

#endif //OPERATOR
