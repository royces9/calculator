#ifndef FUNCS
#define FUNCS

double op(double a, double b, char o);
double ops(double a, char o);
void exec_num(stint *num, char ch);
int funcfind(char buffer[]);
int charfind(char buffer[], stint *num, stchar *ch, double ans, vari *var, int *tok, int *start, char input[], int *error);
int varcheck(vari *list, char inp[]);

#endif //FUNCS
