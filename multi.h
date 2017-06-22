#ifndef MULTI
#define MULTI

double vartypeset(vari *var, char inp[]);
double deri(char inp[10][256], vari *var, int *error);
double inte(char inp[10][256], vari *var, int *error);
double solve(char inp[10][256], vari *var, int *error);
void sep(char *inp, int *start, char sepa[10][256]);

#endif //MULTI
