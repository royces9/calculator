#ifndef STRUCON
#define STRUCON

#define SIN 'a'
#define COS 'b'
#define TAN 'c'
#define LN 'd'
#define LOG 'e'
#define SQRT 'f'
#define ASIN 'g'
#define ACOS 'h'
#define ATAN 'i'

# define NF 19 //Number of functions+constants       

static const char FUNCTIONS[NF][20] = {
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

  "derivative(",
  "integral(",
  "solve("
};

enum functionEnums{
  eQuit,
  eClear,
  eList,
  eHelp,
  ePi,
  eE,
  eAns,
  eSin,
  eCos,
  eTan,
  eLn,
  eLog,
  eSqrt,
  eAsin,
  eAcos,
  eAtan,
  eDeri,
  eInte,
  eSolve
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

#endif //STRUCON
