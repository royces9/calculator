#ifndef SYA
#define SYA

error_return sya(char *inp, vari *var);  
void errorReport(int8_t error);

int8_t checkNumbers(char *input);
int checkOperator(char *a, char b);

int8_t checkType(char a);

#endif //SYA
