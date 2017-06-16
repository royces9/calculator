#ifndef MULTI
#define MULTI

void set_var(char set[], char name[], char value[]);
double deri(char *inp, vari *var);
double inte(char *inp, vari *var);
void sep(char *inp, int *start, char sepa[10][1024]);
double multifunc(int type, char inp[], int *i, vari *var);

#endif //MULTI
