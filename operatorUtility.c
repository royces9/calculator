#include "operatorUtility.h"

const char FUNCTION_LIST[FUNCTION_COUNT][20] = {
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
  "factorial(",

  "derivative(",
  "integral(",
  "solve(",

  "zeros(",
  "ones(",
  "eye(",

  "run(",
  "print("
};


const char OPERATOR_LIST[OPERATOR_COUNT][5] = {
  "+",
  "-",
  ".*",
  "./",

  "^",
  "=",
  "(",
  ")",

  "*",
  "/",

  "<",
  ">",
  "<=",
  ">=",

  "!=",
  "==",

  "&&",
  "||",
  "~"
};


const int operatorPrecedence[OPERATOR_COUNT] = {
  6,
  6,
  5,
  5,

  4,
  4,
  16,
  15,

  5,
  5,

  15,
  8,
  8,
  8,

  8,
  9,

  9,
  11,
  12,
}; //~is not implemented at the moment
