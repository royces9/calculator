#ifndef SYA
#define SYA

err_ret sya(char *inp, struct vari *var);  

int chk_num(char *input);
err_ret chk_op(char *a, char b, int *out);

int chk_t(char a);

#endif //SYA
