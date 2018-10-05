#ifndef OPERATOR
#define OPERATOR


int search_fun(char *buffer);
int search_op(char *buffer);
//operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration);
err_ret ex_num(numberStack *num, vari *var, operatorStruct *ch);

matrix *mat_one(matrix *a, operatorStruct *ch, err_ret *error);
matrix *mat_two(matrix *a, matrix *b, operatorStruct *ch, err_ret *error);

err_ret find_fun(char *buffer, numberStack *num, operatorStack *ch, vari *var, int8_t *tok, uint16_t *iterator, char *input);
err_ret find_op(char *buffer, numberStack *num, operatorStack *ch, vari *var, int8_t *tok);

char **separateMatrix(char *input, uint16_t length, err_ret *error);
matrix *extractMatrix(vari *var, uint16_t *iterator, char *input, err_ret *error);

void help_print(void);

#endif //OPERATOR
