#ifndef FUNCS
#define FUNCS

typedef struct{
  char name[256][256];
  double value[256];
  int occ;
  int count;
} vari;

double op(double a, double b, char o);
double ops(double a, char o);
void exec_num(stint* num, char ch);
int funcfind(char buffer[]);
int charfind(char buffer[], stint* num, stchar* ch, double ans, vari* var, int* tok, int* start, char* input);
int varcheck(vari* list, char inp[]);

#endif //FUNCS
