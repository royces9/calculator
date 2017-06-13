#ifndef STACK
#define STACK

typedef struct{
  double stk[1024];
  int top;
  int occ;
} stint;

typedef struct{
  char stk[1024];
  int top;
  int occ;
} stchar;

void pushn(double inp, stint* st);
double popn(stint* st);
void pushch(char inp, stchar* st);
char popch(stchar* st);

#endif //STACK
