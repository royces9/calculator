#include "strucon.h"

const char FUNCTIONS[NF][20] = {
  "quit",
  "clear",
  "list",
  "help",

  "pi",
  "e",
  "ans",

  "sin(",
  "cos(",
  "tan(",
  "ln(",
  "log(",
  "sqrt(",
  "asin(",
  "acos(",
  "atan(",

  "integral(",
  "derivative("
};

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

typedef struct{
  char name[256][256];
  double value[256];
  int occ;
  int count;
} vari;
