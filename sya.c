#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "operator.h"
#include "stack.h"
#include "sya.h"


//shunting yard algorithm
error_return sya(char *input, vari *var) {

  //stack for output numbers
  numberStack *out = newNumberStack();

  //stack for operators
  operatorStack operatorStack = newOperatorStack();

  //iterators
  int i = 0, j = 0, k = 0;

  //error checking int
  error_return error = 0;

  //length of input
  int length = 0;

  //count for the number of parentheses and brackets
  int parenthesisCount = 0; 
  int bracketCount = 0;

  //to check if the '-' char is subtraction or a negative
  int negativeCheck = 0;

  //flag for setVariable, to check whether
  //variable is saved in var or not
  int8_t variableExist = 0;

  //string for strtod function, unused
  char *str2d = NULL;

  //Error checking
  //check that the number of left end and right end parentheses are the same
  for(length = 0; input[length]; ++length) {
    //increment counter when the current char is a left end or right end parenthese
    parenthesisCount += (input[length] == '(');
    parenthesisCount -= (input[length] == ')');

    bracketCount += (input[length] == '[');
    bracketCount -= (input[length] == ']');
  }

  
  if(parenthesisCount || bracketCount) {
    return error = -3;
  }


  //if input string ends in an operator, return error
  if(strchr(".[,+-/*^(=&|~<>",input[length-1])) {
    return error = -4;
  }

  //buffers for characters and operators
  char *bufferLetters = calloc(length + 1, sizeof(*bufferLetters));
  char *bufferOper = calloc(length + 1, sizeof(*bufferOper));

  //name of variable if assignment
  char *variableAssign;

  int8_t type[length+1];
  for(int l = 0; input[l]; ++l){
    type[l] = checkType(input[l]);

    if(type[l] == 3){
      type[l + 1] = checkType(input[l + 1]);
      type[l] = type[l + 1];
      ++l;
    }
  }

  type[length] = 0;

  //main loop
  //iterators through the input string, apply shunting-yard algorithm
  for(i = 0; input[i]; ++i) {
    switch(type[i]){

    case 1: //alpha numerics
      k = 0;
      bufferLetters[j++] = input[i]; //put all consecutive alphanumeric characters in a buffer

      //is true if it's a valid number/variable name
      if((type[i+1] == 2) || (type[i+1] == 0)){
	bufferLetters[j] = '\0';

	if(checkNumbers(bufferLetters)) { //if the buffer is all numbers, it's a number, otherwise a variable
	  pushn(initScalar(strtod(bufferLetters, &str2d), &error), out);

	} else { //check if command is a function or variable
	  if(input[i+1] == '(') {
	    bufferLetters[j++] = '(';
	  }

	  bufferLetters[j] = '\0';
	  error = findFunction(bufferLetters, out, &operatorStack, var, &negativeCheck, &i, input);
	}
	j = 0; //reset counter for buffer
      } //end if

      negativeCheck = 1; //negative check for the '-' char, which can be minus or negative
      break;

      
    case 2: //operator characters
      j = 0;
      bufferOper[k++] = input[i]; //all consecutive operator characters put into a buffer

      //assumes operators are only two characters wide, checks the current char and the next to see if it's a
      //valid operator, if it is not, then go into the if and find the correct operator in findOperator
      if(checkOperator(bufferOper, input[i+1]) == OPERATOR_COUNT) {
	bufferOper[k] = '\0';
	error = findOperator(bufferOper, out, &operatorStack, var, &negativeCheck); //find the corresponding operator
	k = 0;
      }
      break;

      
    case 3: //'.'
      if(type[i+1] == 2){
	char matrixOper[3] = {input[i], input[i+1], 0};
	error = findOperator(matrixOper, out, &operatorStack, var, &negativeCheck);
	++i;
	j = 0;
      } else if(type[i+1] == 1){
	bufferLetters[j++] = '.';

      } else if(type[i+1] == 0){
	error = -4;
      }

      break;

      
    case 4: //"[]"
      pushn(extractMatrix(var, &i, input, &error), out);
      break;

      
    case -1:
      error = -4;
      break;

      
    }//end of switch
    if((error < 0) || (error == 1)) { //break if error or quit
      freeNumberStack(out);
      free(bufferLetters);
      free(bufferOper);
      return error;
    }
  }//end of for

  free(bufferLetters);
  free(bufferOper);

  while(out->occ && operatorStack.occ) { //while the operator and number stack are occupied, keep executing
    execNum(out, var, popch(&operatorStack), &error);

    if(error){
      freeNumberStack(out);
      return error;
    }
  }


  if(var->ans->size != NULL){
    free(var->ans->size);
    free(var->ans->elements);

    var->ans->size = NULL;
    var->ans->elements = NULL;
  }

  
  //copy out->stk[0] to var->ans
  if(out->stk[0]->size != NULL){
    var->ans->length = out->stk[0]->length;
    var->ans->dimension = out->stk[0]->dimension;

    var->ans->elements = malloc(sizeof(*var->ans->elements) * var->ans->length);
    memcpy(var->ans->elements, out->stk[0]->elements, sizeof(*var->ans->elements) * var->ans->length);

    var->ans->size = malloc(sizeof(*var->ans->size) * (var->ans->dimension + 1));
    memcpy(var->ans->size, out->stk[0]->size, sizeof(*var->ans->size) * (var->ans->dimension + 1));

  } else{
    error = -5;
  }

  //free everything in the numberStack
  freeNumberStack(out);

  return error;
}


//print out errors if there are any
void errorReport(error_return error) {
  if(error < -1){ //error codes for -2 or lower
    printf("\nError:\n");
    switch(error) {

    case -2: printf("Incorrect number of function arguments"); break;
    case -3: printf("Mismatched parenthesis"); break;
    case -4: printf("Invalid expression"); break;
    case -5: printf("Invalid function or variable name"); break;
    case -6: printf("Malloc error"); break;
    case -7: printf("Invalid operator"); break;
    case -8: printf("File does not exist"); break;
    case -9: printf("Mismatched quotation marks"); break;
    case -10:printf("Matrix dimensions do not match."); break;
    case -11:printf("Out of matrix bounds."); break;
    default: break;
    }
  printf("\n\n");
  }
}


//check if the string is a number/variable
error_return checkNumbers(char *input) {
  for(int i = 0; i  < strlen(input); ++i) {
    if(input[i] < '0' && input[i] != '.' || input[i] > '9' || !input[i]) {
      return 0;
    }
  }
  return 1;
}


//check if the two chars together make an operator 
int checkOperator(char *a, char b) {
  int length = strlen(a);
  char buffer[length + 3];
  strcpy(buffer, a);
  buffer[length] = b;
  buffer[length + 1] = '\0';
  
  return searchOperatorArray(buffer);
}


//checks the type of character
int8_t checkType(char a) {
  switch(a) {
    //alpha numeric is 1
    //operators are 2
    //'.' is 3, can be either operator or alpha
    //"[]" is 4, matrix operator
    //misc characters are 0, just ignore em    
    //nonsupported characters are -1?
  case '0' ... '9':
  case 'a' ... 'z':
  case 'A' ... 'Z':
  case '_':
    return 1;

  case '^':
  case '(':
  case '*':
  case '/':
  case '-':
  case '+':
  case ')':
  case '=':
  case '>':
  case '<':
  case '!':
  case '&':
  case '|':
  case '~':
    return 2;

  case '.':
    return 3;

  case '[':
  case ']':
    return 4;
    
  case '\t':
  case '\n':
  case ';':
  case ' ':
    return 0;

  default:
    return -1;
  }
}


//checks if = and == are in the same spot, relies on strchr and strstr to
//linearlly check from the beginning of the string
int8_t isAssign(char *input) {
  return (strchr(input, '=') == (strstr(input, "=="))) ? 0 : 1;
}
