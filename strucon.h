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
#define FLOOR 'j'
#define CEIL 'k'
#define ROUND 'l'
#define MIN 'm'
#define MAX 'n'
#define AVG 'o'

# define NF 25 //Number of functions+constants       

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
  "floor(",
  "ceil(",
  "round(",
  "min(",
  "max(",
  "avg(",

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
  eFloor,
  eCeil,
  eRound,
  eMin,
  eMax,
  eAvg,
  
  eDeri,
  eInte,
  eSolve
};

typedef struct{
  char operator;
  int argNo;
} operatorStruct;

typedef struct{
  double stk[1024];
  int top;
  int occ;
} numberStack;

typedef struct{
  operatorStruct stk[1024];
  int top;
  int occ;
} operatorStack;

typedef struct{
  char name[256][256];
  double value[256];
  int occ;
  int count;
} vari;

#endif //STRUCON
