#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "operator.h"
#include "multi.h"
#include "file.h"


//search in FUNCTION_LIST
int searchFunctionArray(char *buffer) {
  for(int i = 0; i < FUNCTION_COUNT; ++i) {
    if(!strcmp(FUNCTION_LIST[i], buffer)) {
      return i;
    }
  }
  return FUNCTION_COUNT;
}


//search in OPERATOR_COUNT
int searchOperatorArray(char *buffer) {
  for(int i = 0; i < OPERATOR_COUNT; ++i) {
    if(!strcmp(OPERATOR_LIST[i], buffer)) {
      return i;
    }
  }
  return OPERATOR_COUNT;
}


//set the operatorStruct, kinda like a constructor
operatorStruct setOpStack(const char *operator, int argNo, int precedence, int enumeration) {
  operatorStruct out;
  strcpy(out.operator, operator);
  out.precedence = precedence;
  out.argNo = argNo;
  out.enumeration = enumeration;
  return out;
}


//executes either one argument function or two argument function
void execNum(numberStack *num, operatorStruct ch) {
  matrix *a, *b;
  //temp error
  int *error = 0;
  switch(ch.argNo) {
  case 1:
    a = popn(num);
    pushn(matrixOneArg(a, ch), num);
    break;

  case 2:
    b = popn(num);
    a = popn(num);
    pushn(matrixTwoArg(a, b, ch, error), num);
    break;

  default:
    break;
  }
}


//factorial function
element factorial(element a) {
  a = floor(a);
  if(a == 0) {
    return 1;
  }
  return a == 1 ? 1 : a*factorial(a-1);
}

matrix *matrixOneArg(matrix *a, operatorStruct ch, int *error){
  matrix *out;

  freeMatrix(a);
  return out;
}

matrix *matrixTwoArg(matrix *a, matrix *b, operatorStruct ch, int *error){
  matrix *out;

  freeMatrix(a);
  freeMatrix(b);
  return out;
}


//returns value from one argument functions
element oneArg(element a, int o) {
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
  default: return a;
  }
}


//returns value from two argument function
element twoArg(element a, element b, int o) {
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

int findFunction(char *buffer, numberStack *num, operatorStack *ch, vari *var, int *tok, int *start, char *input) {
  char **separatedString;
  int i = searchFunctionArray(buffer), error = 0;
  operatorStruct operator;
  matrix *out;

  switch(i) {
  case eQuit: //quit
    return 1;

  case eClear: //clear
    freeVari(var);
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n\n");
    return -1;

  case eList: //list
    if(var->occ) {
      printf("\nVariable List:\n");
      for(int j = 0; j <= var->count; ++j) {
	printf("%s =", var->name[j]);
	printMatrix(*var->value[j]);
      }
      printf("\n");
    } else {
      printf("\nNo variables set\n\n");
    }
    return -1;

  case eHelp: //help
    helpPrint();
    return -1;

  case ePi:
    pushn(initScalar(M_PI), num);
    *tok = 1;
    return 0;

  case eE:
    pushn(initScalar(M_E), num);
    *tok = 1;
    return 0;

  case eAns:
    { //because ans scope is in main, make a copy of ans that can be free'd
      matrix *copy = malloc(sizeof(*copy));
      copyMatrix(copy, var->ans);
      pushn(copy, num);
      *tok = 1;
    }
    return 0;

  case eSin:
  case eCos:
  case eTan:
  case eLn:
  case eLog:
  case eSqrt:
  case eAsin:
  case eAcos:
  case eAtan:
  case eFloor:
  case eCeil:
  case eRound:
  case eFactorial:
  case eMin:
  case eMax:
  case eAvg:
    pushch(setOpStack(FUNCTION_LIST[i], 1, 2, i), ch);
    *tok = 0;
    return 0;

  case eDeri:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(deri(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eInte:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(inte(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eSolve:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(solve(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eZeros:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(zeros(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eOnes:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(ones(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eEye:
    *tok = 0;
    return error;

  case eRun:
    separatedString = separateString(input, "()", '\0', start, &error);
    error = runFile(separatedString, var, out);
    pushn(out, num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case ePrint:
    separatedString = separateString(input, "()", ',', start, &error);
    printLine(separatedString, var, &error);
    freeDoubleArray(separatedString);
    return error;

  case FUNCTION_COUNT: //variables
    {
      int k = varcheck(var, buffer);
      if(k >= 0) {
	matrix *varCopy = malloc(sizeof(*varCopy));
	pushn(copyMatrix(varCopy,var->value[k]), num);
	*tok = 1;
	return 0;
      }
    }
    break;

  default:
    break;

  }//end of switch

  return -5;
}



int findOperator(char *buffer, numberStack *num, operatorStack *oper, vari *var, int *tok) {
  int i = searchOperatorArray(buffer);
  int error = 0;

  /*                                                                                                                  
  **Precedence values for operators: Reference wiki page of C/C++ operators
  **1                                                
  **2 f(x)-calls
  **3
  **4 ^ !
  **5
  **6 + -
  **7
  **8 < <= > >=
  **9 == !=
  **10
  **11 &&
  **12 ||
  **13
  **14
  **15 parens
  **16 =
*/
  switch(i) {
  case eSubtract:
    if(*tok == 1) {
      while((oper->stk[oper->top].precedence <= 6) && oper->occ == 1) {
	execNum(num, popch(oper));
      }
      pushch(setOpStack("+", 2, 6, eAdd), oper);
    }

    *tok = 0;
    pushch(setOpStack("*", 2, 5, eMultiply), oper);
    pushn(initScalar(-1), num);
    break;

  case eExponent:
    *tok = 0;
    pushch(setOpStack("^", 2, 4, eExponent), oper);
    break;

  case eLeftParen:
    *tok = 0;
    pushch(setOpStack("(", 1, 15, eLeftParen), oper);
    break;

  case eRightParen:
    do {
      execNum(num, popch(oper));
    } while(strcmp(oper->stk[oper->top].operator, "(") && oper->occ == 1);

    *tok = 1;
    popch(oper);
    break;


  case eAssign:
    break;

  case eAdd:
  case eMultiply:
  case eDivide:
  case eLess:
  case eGreater:
  case eLessEqual:
  case eGreaterEqual:
  case eNotEqual:
  case eEqual:
  case eAnd:
  case eOr:
    while((oper->stk[oper->top].precedence <= operatorPrecedence[i]) && (oper->occ == 1)) {
      execNum(num, popch(oper));
    }
    *tok = 0;
    pushch(setOpStack(buffer, 2, operatorPrecedence[i], i), oper);
    break;

  default:
    return -7;
  }


  return 0;
}


//separate a matrix, accounting for sub matrices as input in a matrix
//[[1, 2], 3] or something like that
matrix *separateMatrix(char *input, int *offset, vari *var, int *error ){
  int length = 0;
  matrix *out = malloc(sizeof(*out));

  //copy variable struct to not write over ans
  vari temp = *var;

  //counter for brackets, in order:
  //()[]
  int bracketCount[4] = {0, 0, 0, 0};
  char bracket[4] = "()[]";

  for(;input[length];++length){
    for(int i = 0; i < 4; ++i){
      bracketCount[i] += (input[length] == bracket[i]);
    }

    //total number of ( and ); and; [ and ], are the same
    //and the current character is a comma
    if(((bracketCount[0] == bracketCount[1]) && (bracketCount[2] == bracketCount[3]))
       && (input[length] == ',')){
      break;
    }
  }

  char *cutInput = malloc(sizeof(*cutInput) * length);
  strncpy(cutInput, input, length);
  *error = sya(cutInput, &temp);
  copyMatrix(out, &temp.ans);

  *offset += (length + 1);

  free(temp.ans.size);
  free(temp.ans.elements);
  return out;
}

matrix *extractMatrix(vari *var, int *start, char *input, int *error){
  matrix *out;

  //input is initially the full string, start is the index for the first '['
  //this changes input to be the first char after the first '['
  input += (*start + 2);

  //find where the matrix declaration ends
  //count brackets until they match
  int leftBracketCount = 0, rightBracketcount = 0;
  for(length = 0; input[length]; ++length){
    leftBracketCount += (input[length] == '[');
    rightBracketCount += (input[length] == ']');
    if(leftBracketCount == rightBracketCount){
      break;
    }
  }

  //check that the bracket count is correct
  if(leftBracketCount != rightBrackCount){
    *error = -4;
    return NULL;
  }

  ++length;
  
  //the string that will contain every character that contains elements of the matrix
  char *matrixString = malloc(sizeof(*matrixString) * length);
  strncpy(matrixString, input, sizeof(*matrixString) * length);

  //number stack for creating the matrix
  numberStack numStk = newNumberStack();
  //operator stack for concatenating
  operatorStack opStk = newOperatorStack();

  int offset = 0;

  for(int i = 0; i < length; ++i){
    if((matrixString[i] == ',') || (matrixString[i] == ';')){
      operatorStruct sh;
      sh.precedence = ((matrixString[i] == ',') + 1);
      pushch(sh, &opStk);
    } else{
      pushn(separateMatrix(matrixString + offset, &offset, var, error), &stk);
      i += offset;
      if(*error) return NULL;
    }
    if(opStk.stk[top].precedence == 2){
      matrix *b = popn(&numStk);
      matrix *a = popn(&numStk);
      concatMatrix(a, b, 2, error);
      if(*error) return NULL;
    }
  }

  return out;
}


int varcheck(vari *list, char input[]) {
  if(list->occ == 0) {
    return -1;
  }

  for(int i = 0; i<=list->count; ++i) {
    if(!strcmp(input, list->name[i])) {
      return i;
    }
  }

  return -2;
}


void freeDoubleArray(char **input) {
  int i = 0;
  for(i = 0; strcmp(input[i], ""); ++i) {
    free(input[i]);
  }
  free(input[i]);
  free(input);
}


void helpPrint() {
  printf("quit - quit program\n");
  printf("list - list variables\n");
  printf("clear - clear variables\n\n");

  printf("derivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\n");

  printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\nb - ending point\n   n - number of partitions (composite Simpson's rule, odd n is equivalent to n-1)\n\n");

  printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");

  printf("run(file)\n   file - path to a text file\n   This function parses each line of the file as if it were entered into the console directly, with the exception of \"while\", \"if/else\" and \"end\".\n    \"while\" - loops until the statement inside the \"while\"'s conditional is false. The inside is executed as if it were entered into the console directly. There may be floating point round off errors.\n   \"if/else\" - Executes the block of lines inside the \"if\"'s conditional if the statement is true, Otherwise it will execute the \"else\" block.\n   '#' at the beginning of\
 a line comments out a line\n   ';' at the end of a line suppresses output\n\n");
}









