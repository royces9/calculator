#ifndef MULTI
#define MULTI

double vartypeset(vari *var, char inp[]);
double deri(char *inp, vari *var, int *error);
double inte(char *inp, vari *var, int *error);
void sep(char *inp, int *start, char sepa[10][1024]);
double multifunc(int type, char inp[], int *i, vari *var, int *error);

#endif //MULTI
