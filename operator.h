#ifndef OPERATOR
#define OPERATOR

int search_str(char *buffer, char const *const *const list);
err_ret ex_num(struct stack *num, struct vari *var, struct oper *ch);

err_ret mat_one(struct matrix *a, struct oper *ch, struct matrix **out);
err_ret mat_two(struct matrix *a, struct matrix *b, struct oper *ch, struct matrix **out);

err_ret find_fun(char *buffer, struct stack *num, struct stack *ch, struct vari *var, int *tok, int *iter, char *input);
err_ret find_op(char *buffer, struct stack *num, struct stack *ch, struct vari *var, int *tok);

char **sep_mat(char *input, uint16_t length, err_ret *error);
err_ret ext_mat(struct vari *var, int *iter, char *input, struct matrix **out);

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
