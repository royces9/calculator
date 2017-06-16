#ifndef STRUCON
#define STRUCON

#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442

#define E 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260595630738132328627943490763233829880753195251019011573

#define SIN 'a'
#define COS 'b'
#define TAN 'c'
#define LN 'd'
#define LOG 'e'
#define SQRT 'f'
#define ASIN 'g'
#define ACOS 'h'
#define ATAN 'i'

# define NF 18 //Number of functions+constants       

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
