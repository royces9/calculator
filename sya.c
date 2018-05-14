#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operator.h"
#include "stack.h"
#include "sya.h"


//shunting yard algorithm
int sya(char *input, vari *var) {
  numberStack out = newNumberStack(); //stack for output numbers
  operatorStack oper = newOperatorStack(); //stack for operators

  int i = 0, j = 0, k = 0; //iterators
  int error = 0; //error checking int
  int length = 0; //number of parens and length of input
  int parenthesisCount = 0;
  int bracketCount = 0;
  
  int check = 0; //for if an assignment occurs
  int negativeCheck = 0; //to check if the '-' char is subtraction or a negative
  char varset = 0, *str2d = NULL; //string for strtod function, unused


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
  if(strchr("[,+-/*^(=&|~<>",input[length-1])) {
    return error = -4;
  }


  //buffers for characters and operators
  char bufferLetters[length], bufferOper[length];

  //name of variable if assignment
  char *variableAssign;

  int type[length+1];
  for(int l = 0; input[l]; ++l){
    type[l] = checkType(input[l]);
  }

  type[length] = 0;
  
  //main loop
  //iterators through the input string, apply shunting-yard algorithm
  for(i = 0; input[i]; ++i) {
    //printf("%c\n", input[i]);
    switch(type[i]){
      
    case 1: //alpha numerics
      k = 0;
      bufferLetters[j++] = input[i]; //put all consecutive alphanumeric characters in a buffer
      if(((type[i+1] == 2) || (type[i+1] == 0)) && (input[i+1] != '\n')){ //is true if it's a valid number/variable name
	bufferLetters[j] = '\0';

	if(checkNumbers(bufferLetters)) { //if the buffer is all numbers, it's a number, otherwise a variable
	  pushn(initScalar(strtod(bufferLetters, &str2d)), &out);
	} else if(!varset && isAssign(input)) { //checks if the command is an assignment
	  check = varcheck(var, bufferLetters); //checks that the variable exists
	  varset = 1; //flag for assignment at the end of the sya loop

	  variableAssign = malloc(sizeof(*variableAssign) * (j + 1));
	  strcpy(variableAssign, bufferLetters);

	} else { //check if command is a function or variable
	  if(input[i+1] == '(') {
	    bufferLetters[j++] = '(';
	  }

	  bufferLetters[j] = '\0';
	  error = findFunction(bufferLetters, &out, &oper, var, &negativeCheck, &i, input);
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
      if(checkOper(input[i], input[i+1]) == OPERATOR_COUNT) {
	bufferOper[k] = '\0';
	error = findOperator(bufferOper, &out, &oper, var, &negativeCheck); //find the corresponding operator
	k = 0;
      }
      break;

      
    case 3: //'.'

      if(type[i+1] == 2){
	char matrixOper[3] = {input[i], input[i+1], 0};
	error = findOperator(matrixOper, &out, &oper, var, &negativeCheck);
	++i;

      } else if(type[i+1] == 1){
	bufferLetters[j++] = '.';
      }

      break;

      
    case 4: //"[]"
      pushn(extractMatrix(var, &i, input, &error), &out);
      break;

    case -1:
      error = -4;
      break;

    }//end of switch

    if((error < 0) || (error == 1)) { //break if error or quit
      return error;
    }
  }//end of for

  while(out.occ && oper.occ) { //while the operator and number stack are occupied, keep executing
    execNum(&out, popch(&oper), &error);
  }
  if(error){
    return error;
  }

  if(var->ans.elements != NULL){
    free(var->ans.elements);
  }
  if(var->ans.size != NULL){
    free(var->ans.size);
  }

  
  //copy out.stk[0] to var->ans
  var->ans.length = out.stk[0]->length;
  var->ans.dimension = out.stk[0]->dimension;

  var->ans.elements = malloc(sizeof(*var->ans.elements) * var->ans.length);
  memcpy(var->ans.elements, out.stk[0]->elements, sizeof(*var->ans.elements) * var->ans.length);
  
  var->ans.size = malloc(sizeof(*var->ans.size) * (var->ans.dimension + 1));
  memcpy(var->ans.size, out.stk[0]->size, sizeof(*var->ans.size) * (var->ans.dimension + 1));

  //free out.stk[0]
  freeMatrix(out.stk[0]);


  if(varset) {
    //set variable if there was an assignment
    //check that the newly assigned variable hasn't been assigned before
    //free it if it has
    error = setVariable(var, variableAssign, check);
  }

  return error;
}


//print out errors if there are any
void errorrep(int error) {
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
    default: break;
    }
  printf("\n\n");
  }
}


//check if the string is a number or not
int checkNumbers(char *input) { //check if the input string is a number
  for(int i = 0; i  < strlen(input); ++i) {
    if(input[i] < '0' && input[i] != '.' || input[i] > '9' || !input[i]) {
      return 0;
    }
  }
  return 1;
}


//check if the two chars together make an operator 
int checkOper(char a, char b) {
  char buffer[2] = {a, b};
  //if b is '\0', end of string
  //if(!b) return OPERATOR_COUNT;
  return searchOperatorArray(buffer);
}


//checks the type of character
int checkType(char a) {
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
int isAssign(char *input) {
  return (strchr(input, '=') == (strstr(input, "=="))) ? 0 : 1;
}
