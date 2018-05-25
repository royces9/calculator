#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "functions.h"
#include "operator.h"
#include "multi.h"
#include "file.h"
#include "sya.h"


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


//executes either one argument function or two argument function
void execNum(numberStack *num, vari *var, operatorStruct ch, error_return *error) {
  matrix *a = NULL, *b = NULL;

  switch(ch.argNo) {
  case 1:
    a = popn(num);
    if(a == NULL){
      *error = -15;
      return;
    }
    pushn(matrixOneArg(a, ch, error), num);
    break;

  case 2:
    b = popn(num);
    a = popn(num);
    if((a == NULL) || (b == NULL)){

      if(a != NULL){
	freeMatrix(a);
      }

      if(b != NULL){
	freeMatrix(b);
      }

      *error = -10;
      return;
    }

    pushn(matrixTwoArg(a, b, ch, error), num);
    break;

  case 3:
    b = popn(num);
    a = popn(num);
    pushn(assign(a, b, var, error), num);
    freeMatrix(b);
    break;

  default:
    break;
  }
}


matrix *matrixOneArg(matrix *a, operatorStruct ch, error_return *error){
  matrix *out;
  error_return check = 0;
  //check if the enumeration is one of the function in oneArg
  oneArg(0, ch.enumeration, &check);

  //check remains 0, is in oneArg
  if(!check){
    int j = 0;

    int *size = malloc(sizeof(*size) * (a->dimension + 1));
    memcpy(size, a->size, sizeof(*size) * (a->dimension + 1));

    out = initMatrix(size, a->dimension, error);
    free(size);

    for(int i = 0; i < out->length; ++i){
      out->elements[i] = oneArg(a->elements[i], ch.enumeration, error);
    }
  } else{
    switch(ch.enumeration){
    case eEye: out = eye(a, error); break;
    case eSize: out = getSize(a, error); break;
    case eTranspose: out = transposeMatrix(a, error); break;
    case eMax: out = max(a, error); break;
    case eMin: out = min(a, error); break;
    case eAvg: out = avg(a, error); break;
    default: out = copyMatrix(a, error); break;
    }
  }
  freeMatrix(a);
  return out;
}


matrix *matrixTwoArg(matrix *a, matrix *b, operatorStruct ch, error_return *error){
  matrix *out = NULL;
  error_return check = 0;
  twoArg(0, 0, ch.enumeration, &check);
  //check if inputs are scalar
  int aScalar = isScalar(a);
  int bScalar = isScalar(b);

  if(((aScalar + bScalar) > 0) && check){

    //if the enum is for a matrix operation
    switch(ch.enumeration){
    case eMultiplyMatrix:
      check = 0;
      ch.enumeration = eMultiply;
      break;

    case eDivideMatrix:
      check = 0;
      ch.enumeration = eDivide;
      break;
    }
  }

  //if ch.enumeration is not in twoArg, check will be 1
  if(!check){
    switch(aScalar + bScalar){
    case 0: //neither is a scalar
      if(compareSize(a->size, b->size, a->dimension, b->dimension)){
	out = initMatrix(a->size, a->dimension, error);

	for(int i = 0; i < a->length; ++i){
	  out->elements[i] = twoArg(a->elements[i], b->elements[i], ch.enumeration, error);
	}

      } else{
	*error = -10;
      }
      break;
      
    case 1: //only a or b is a scalar

      if(aScalar){
	out = copyMatrix(b, error);
	for(int i = 0; i < out->length; ++i){
	  out->elements[i] = twoArg(a->elements[0],b->elements[i], ch.enumeration, error);
	}

      } else{
	out = copyMatrix(a, error);
	for(int i = 0; i < out->length; ++i){
	  out->elements[i] = twoArg(a->elements[i], b->elements[0], ch.enumeration, error);
	}
      }
      break;

    case 2: //a and b are scalars
      out = initScalar(twoArg(a->elements[0], b->elements[0], ch.enumeration, error), error);
      break;

    default:
      *error = -2;
      break;

    }
  } else{
    switch(ch.enumeration){
    case eMultiplyMatrix: out = multiplyMatrix(a, b, error); break;
    case eExponentMatrix: out = exponentMatrix(a, b, error); break;
    case eDivideMatrix: out = divideMatrix(a, b, error); break;
    case eReference: out = reference(a, b, error); break;
    default: *error = -10; break;
    }
  }

  freeMatrix(a);
  freeMatrix(b);

  return out;
}


error_return findFunction(char *buffer, numberStack *num, operatorStack *ch, vari *var, int *tok, int *start, char *input) {
  char **separatedString = NULL;
  int i = searchFunctionArray(buffer);
  error_return error = 0;
  operatorStruct operator;

  switch(i) {
  case eQuit: //quit
    return 1;

  case eClear: //clear
    if(var->occ != 0){
      for(int i = 0; i <= var->count; ++i){
	freeMatrix(var->value[i]);
      }
    }
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
    pushn(initScalar(M_PI, &error), num);
    *tok = 1;
    return 0;

  case eE:
    pushn(initScalar(M_E, &error), num);
    *tok = 1;
    return 0;

  case eAns:
    { //because ans is not a heap variable, make a
      //copy of it and push the copy
      pushn(copyMatrix(var->ans, &error), num);
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
  case eEye:
  case eSize:
  case eTranspose:
    pushch(initOperatorStruct(FUNCTION_LIST[i], 1, 2, i), ch);
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

  case eRand:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(randMatrix(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eLinspace:
    separatedString = separateString(input, "()", ',', start, &error);
    pushn(linspace(separatedString, var, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case eRun:
    separatedString = separateString(input, "()", '\0', start, &error);
    error = runFile(separatedString, var);

    //copy ans matrix because it's not a heap variable
    pushn(copyMatrix(var->ans, &error), num);
    freeDoubleArray(separatedString);
    *tok = 0;
    return error;

  case ePrint:
    separatedString = separateString(input, "()", ',', start, &error);
    error = printLine(separatedString, var);
    freeDoubleArray(separatedString);
    return error;

  case FUNCTION_COUNT: //variables
    {
      int varLen = strlen(buffer);
      int k = 0;
      matrix *out = NULL;

      if(buffer[varLen - 1] == '('){

	buffer[varLen - 1] = '\0';
	separatedString = separateString(input, "()", ',', start, &error);

	k = varcheck(var, buffer);

	out = extractValue(buffer, separatedString, k, var, &error);

	if(out != NULL){
	  pushn(var->value[k], num);
	  pushn(out, num);

	  pushch(initOperatorStruct("r", 2, 0, eReference), ch);
	}

	freeDoubleArray(separatedString);

      } else{
	k = varcheck(var, buffer);
	if(k >= 0) {
	  var->value[k]->variable = 1;
	  pushn(var->value[k], num);
	  *tok = 1;

	} else{
	  if(k == -1){
	    k = 0;

	  } else if(k == -2){
	    k = ++var->count;

	  } else{
	    error = -5;

	    k = var->count + 1;
	  }

	  //if assignment goes wrong, the variable name gets malloc'd
	  //but doesn't get assigned to, this ensures that if another
	  //assignment happens, the previous failed assignment is free'd
	  if((var->name[k] != NULL) && (var->value[k]->size == NULL)){
	    free(var->name[k]);
	    free(var->value[k]);
	  }

	  var->name[k] = malloc(sizeof(*var->name[k]) * (varLen + 1));
	  strcpy(var->name[k], buffer);

	  var->value[k] = malloc(sizeof(*var->value[k]));
	  var->value[k]->variable = 1;
	  var->value[k]->size = NULL;
	  var->value[k]->elements = NULL;

	  pushn(var->value[k], num);
	}
      }
    }
    break;

  default:
    error = -5;
    break;

  }//end of switch

  return error;
}


error_return findOperator(char *buffer, numberStack *num, operatorStack *oper, vari *var, int *tok) {
  int i = searchOperatorArray(buffer);
  error_return error = 0;
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
	execNum(num, var, popch(oper), &error);
      }
      pushch(initOperatorStruct("+", 2, 6, eAdd), oper);
    }

    *tok = 0;
    pushch(initOperatorStruct("*", 2, 5, eMultiply), oper);
    pushn(initScalar(-1, &error), num);
    break;

  case eExponentMatrix:
    *tok = 0;
    pushch(initOperatorStruct("^", 2, 4, eExponentMatrix), oper);
    break;
    
  case eExponent:
    *tok = 0;
    pushch(initOperatorStruct(".^", 2, 4, eExponent), oper);
    break;

  case eLeftParen:
    *tok = 0;
    pushch(initOperatorStruct("(", 1, 15, eLeftParen), oper);
    break;

  case eRightParen:
    do {
      execNum(num, var, popch(oper), &error);
    } while(strcmp(oper->stk[oper->top].operator, "(") && oper->occ == 1);

    *tok = 1;
    popch(oper);
    break;


  case eAssign:
    *tok = 0;
    if(oper->stk[oper->top].enumeration == eReference){
      var->assignIndex = popn(num);

      popch(oper);
    }
    pushch(initOperatorStruct("=", 3, 16, eAssign), oper);
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
  case eMultiplyMatrix:
  case eDivideMatrix:
    while((oper->stk[oper->top].precedence <= operatorPrecedence[i]) && (oper->occ == 1)) {
      execNum(num, var, popch(oper), &error);
    }
    *tok = 0;
    pushch(initOperatorStruct(buffer, 2, operatorPrecedence[i], i), oper);
    break;

  default:
    return -7;
  }

  return 0;
}


//separate a matrix, accounting for sub matrices as input in a matrix
//[[1, 2], 3] or something like that
char **separateMatrix(char *input, int delimiter, error_return *error){
  char **out = malloc(sizeof(*out) * (delimiter + 2));

  //counter for brackets, parentheses and brackets:
  int bracketCount[4] = {0, 0};

  //the last index where a ',' or ';' was found
  int currentLength = 0;

  //count of the number of subMatrices
  int subMatrices = 0;

  int j = 0;
  for(; input[j]; ++j){
    switch(input[j]){
    case '(':
      ++bracketCount[0];
      break;
    case ')':
      --bracketCount[0];
      break;
    case '[':
      ++bracketCount[1];
      break;
    case ']':
      --bracketCount[1];
      break;
    case ',':
    case ';':
      if( !( bracketCount[0] || bracketCount[1] ) ){
	out[subMatrices] = malloc(sizeof(**out) * ((j - currentLength) + 2));

	strncpy(out[subMatrices], input + currentLength, j - currentLength);

	out[subMatrices][j - currentLength] = '\0';
	currentLength = j;
	++subMatrices;
      }
      break;

    default: break;
    }
  }

  //j is the length after the for loop runs
  out[subMatrices] = malloc(sizeof(**out) * ((j - currentLength) + 1));
  strncpy(out[subMatrices], input + currentLength, j - currentLength);
  out[subMatrices][j - currentLength] = '\0';

  out[++subMatrices] = NULL;
  return out;
}


int countDelimiter(char *input){
  int out = 0;
  int bracketCount[2] = {0, 0};

  for(int i = 0; input[i]; ++i){
    switch(input[i]){
    case '(':
      ++bracketCount[0];
      break;

    case ')':
      --bracketCount[0];
      break;

    case '[':
      ++bracketCount[1];
      break;

    case ']':
      --bracketCount[1];
      break;

    case ',':
    case ';':
      if(!(bracketCount[0] || bracketCount[1])){
	++out;
      }
      break;

    default: break;
    }
  }

  return out;
}


matrix *extractMatrix(vari *var, int *start, char *input, error_return *error){
  //input is incremented to start at input[*start], which is where
  //the first [ should be
  input += (*start);

  //find where the matrix declaration ends
  //count brackets until they match
  //also get the length of string
  int bracketCount = 0;
  int length = 0;

  for(length = 0; input[length]; ++length){
    bracketCount += (input[length] == '[');
    bracketCount -= (input[length] == ']');

    if(!bracketCount){
      break;
    }
  }

  //check that the bracket count is correct
  if(bracketCount){
    *error = -4;
    return NULL;
  }

  //increment the main loop counter up to the ']' 
  *start += (length);

  //the string that will contain every character that contains elements of the matrix
  char *matrixString = malloc(sizeof(*matrixString) * (length));

  //copy from the first character after the first '['
  strncpy(matrixString, input + 1, sizeof(*matrixString) * (length));

  //replace the end ']' with a '\0'
  matrixString[length-1] = 0;

  if((matrixString[length-2] == ';') || (matrixString[length-2] == ',')){
    free(matrixString);
    *error =  -4;
    return NULL;
  }

  //number stack for creating the matrix
  numberStack *numStk = newNumberStack();

  char **separatedMatrix = separateMatrix(matrixString, countDelimiter(matrixString), error);

  //free matrixString, not needed anymore
  free(matrixString);

  vari *tempVari = copyVari(var, error);
  *error = sya(separatedMatrix[0], tempVari);

  pushn(copyMatrix(tempVari->ans, error), numStk);

  matrix *a, *b, *out;

  for(int i = 1; separatedMatrix[i]; ++i){
    if(separatedMatrix[i][1] == 0){
      *error = -4;
      freeNumberStack(numStk);
      freeVari(tempVari);
      freeDoubleArray(separatedMatrix);
      return NULL;
    }

    switch(separatedMatrix[i][0]){
    case ',':
      *error = sya(separatedMatrix[i] + 1, tempVari);
      a = popn(numStk);

      pushn(concatMatrix(a, tempVari->ans, 1, error), numStk);
      freeMatrix(a);
      break;

    case ';':
      *error = sya(separatedMatrix[i] + 1, tempVari);

      pushn(copyMatrix(tempVari->ans, error), numStk);
      break;
      
    default:
      *error = -10;
      freeNumberStack(numStk);
      freeDoubleArray(separatedMatrix);
      freeVari(tempVari);
      return NULL;
    }
  }

  while(numStk->occ && (numStk->top != 0)){
    b = popn(numStk);
    a = popn(numStk);

    pushn(concatMatrix(a, b, 0, error), numStk);

    freeMatrix(a);
    freeMatrix(b);
  }

  freeVari(tempVari);

  freeDoubleArray(separatedMatrix);

  out = popn(numStk);
  free(numStk);

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
  for(i = 0; input[i]; ++i) {
    free(input[i]);
  }
  free(input[i]);
  free(input);
}


void helpPrint(void) {
  printf("quit - quit program\n");
  printf("list - list variables\n");
  printf("clear - clear variables\n\n");

  printf("derivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\n");

  printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\nb - ending point\n   n - number of partitions (composite Simpson's rule, odd n is equivalent to n-1)\n\n");

  printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");

  printf("run(file)\n   file - path to a text file\n   This function parses each line of the file as if it were entered into the console directly, with the exception of \"while\", \"if/else\" and \"end\".\n    \"while\" - loops until the statement inside the \"while\"'s conditional is false. The inside is executed as if it were entered into the console directly. There may be floating point round off errors.\n   \"if/else\" - Executes the block of lines inside the \"if\"'s conditional if the statement is true, Otherwise it will execute the \"else\" block.\n   '#' at the beginning of\
 a line comments out a line\n   ';' at the end of a line suppresses output\n\n");
}
