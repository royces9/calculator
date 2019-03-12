#ifndef SYA
#define SYA

err_ret sya(char *inp, vari *var);  
void err_rep(int8_t error);

int chk_num(char *input);
int chk_op(char *a, char b, err_ret *error);

int chk_t(char a);

#endif //SYA
