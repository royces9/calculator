#include <math.h>
#include <stdio.h>

#include "operator.h"

//factorial function
element factorial(element a) {
  a = floor(a);
  if(a == 0) {
    return 1;
  }
  return a == 1 ? 1 : a*factorial(a-1);
}


matrix *eye(matrix *a, int *error){
  if(a->dimension != 1){
    *error = -12;
    return NULL;
  }

  int size[2] = {a->elements[0], a->elements[0]};
  matrix *out = initMatrix(size, 2, error);

  int index = 0;
  for(int i = 0; i < a->elements[0]; ++i){
    index = i * (a->elements[0] + 1);
    out->elements[index] = 1;
  }

  return out;
}

//returns value from one argument functions
element oneArg(element a, int o, int *error) {
switch(o) {
 case eSin: return sin(a);
 case eCos: return cos(a);
 case eTan: return tan(a);
 case eLn: return log(a);
 case eLog: return log10(a);
 case eSqrt: return sqrt(a);
 case eAsin: return asin(a);
 case eAcos: return acos(a);
 case eAtan: return atan(a);
 case eFloor: return floor(a);
 case eCeil: return ceil(a);
 case eRound: return round(a);
 case eFactorial: return factorial(a);
 default: *error = 1; return a;
}
}


//returns value from two argument function
element twoArg(element a, element b, int o, int *error) {
switch(o) {
 case eAdd: return a + b;
 case eSubtract: return a - b;
 case eMultiply: return a * b;
 case eDivide: return a / b;
 case eExponent: return pow(a, b);
 case eLess: return a < b;
 case eGreater: return a > b;
 case eLessEqual: return a <= b;
 case eGreaterEqual: return a >= b;
 case eNotEqual: return a != b;
 case eEqual: return a == b;
 case eAnd: return a && b;
 case eOr: return a || b;
}
}
