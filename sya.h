#ifndef SYA
#define SYA

err_ret sya(char *inp, struct vari *var);  
void err_rep(int8_t error);

int chk_num(char *input);
err_ret chk_op(char *a, char b, int *out);

int chk_t(char a);

#endif //SYA
