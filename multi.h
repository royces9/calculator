#ifndef MULTI
#define MULTI

double deri(char* inp, char var, double range[], double delta);
double inte(char* inp, char var, double range[], double delta);
char* sep(char* inp, int* start);
double multifunc(int type, char inp[], int* i);

#endif //MULTI
