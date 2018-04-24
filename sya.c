#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operator.h"
#include "stack.h"
#include "onearg.h"
#include "sya.h"


//shunting yard algorithm
int sya(char *input, matrix *ans, vari *var) {
  numberStack out = newNumberStack(); //stack for output numbers
  operatorStack oper = newOperatorStack(); //stack for operators

  int i = 0, j = 0, k = 0; //iterators
  int error = 0; //error checking int
  int leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0; //number of parens and length of input
  int leftBrackCount = 0, rightBrackCount = 0;
  
  int check = 0; //for if an assignment occurs
  int negativeCheck = 0; //to check if the '-' char is subtraction or a negative
  char varset = 0, *str2d = NULL; //string for strtod function, unused


  //Error checking
  //check that the number of left end and right end parentheses are the same
  for(length = 0; input[length]; ++length) {
    //increment counter when the current char is a left end or right end parenthese
    leftParenthesisCount += (input[length] == '(');
    rightParenthesisCount += (input[length] == ')');

    leftBracketCount += (input[length] == '[');
    rightBracketCount += (input[length] == ']');
  }

  
  if((leftParenthesisCount != rightParenthesisCount) || (leftBracketCount != rightBracketCount)) {
    return error = -3;
  }


  //if input string ends in an operator, return error
  if(strchr("+-/*^(=&|~<>",input[length-1])) {
    return error = -4;
  }

  //buffers for characters and operators
  char bufferLetters[length], bufferOper[length];

  //main loop
  //iterators through the input string, apply shunting-yard algorithm
  for(i = 0; input[i]; ++i) {
    int type = checkType(input[i+1]); //checks the next char for a type defined in checkType function
    switch(input[i]){

      //alphanumeric
    case '0' ... '9':
    case '.':
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
      bufferLetters[j++] = input[i]; //put all consecutive alphanumeric characters in a buffer
      if(((type == 2) || (type == 0)) && (input[i+1] != '\n')){ //is true if it's a valid number/variable name
	bufferLetters[j] = '\0';
	if(checkNumbers(bufferLetters)) { //if the buffer is all numbers, it's a number, otherwise a variable
	  pushn(strtod(bufferLetters, &str2d), &out);
	} else if(isAssign(input) && !varset) { //checks if the command is an assignment
	  check = varcheck(var, bufferLetters); //checks that the variable exists
	  varset = 1; //flag for assignment at the end of the sya loop

	  if(check == -1) { //var struct is empty, adds first variable to the struct
	    strcpy(var->name[0], bufferLetters);
	    var->occ = 1; //sets the struct occupied flag to true
	    var->count = 0;
	    check = 0; //the index of the new variable
	  } else if(check == -2) { //var struct is not empty, but it's a new variable
	    check = ++var->count;
	    strcpy(var->name[check], bufferLetters);
	  }
	} else {
	  if(input[i+1] == '(') {
	    bufferLetters[j++] = '(';
	  }
	  bufferLetters[j] = '\0';
	  error = findFunction(bufferLetters, &out, &oper, *ans, var, &negativeCheck, &i, input);
	} //end else
	j = 0; //reset counter for buffer
      } //end if
      negativeCheck = 1; //negative check for the '-' char, which can be minus or negative
      break;


      //operators
    case '^':

    case '*':
    case '/':

    case '-':
    case '+':

    case '(':
    case ')':

    case '[':
    case ']':

    case '=':
    case '>':
    case '<':

    case '!':
    case '&':
    case '|':
    case '~':
      bufferOper[k++] = input[i]; //all consecutive operator characters put into a buffer

      //assumes operators are only two characters wide, checks the current char and the next to see if it's a
      //valid operator, if it is not, then go into the if and find the correct operator in findOperator
      if(checkOper(input[i], input[i+1]) == OPERATOR_COUNT) {
	bufferOper[k] = '\0';
	error = findOperator(bufferOper, &out, &oper, *ans, var, &negativeCheck); //find the corresponding operator
	k = 0;
      }
      break;


      //other
    case ' ': //skip spaces, new line, semicolon
    case '\t':
    case '\n':
    case ';':
      break;


      //any other characters
    default: //any other characters give errors
      return error = -4;
      
    }//end of switch
    if(error < 0) { //break if error
      return error;
    }
  }//end of for
  
  while(out.occ && oper.occ) { //while the operator and number stack are occupied, keep executing
    execNum(&out, popch(&oper));
  }

  *ans = out.stk[0]; //put the output into ans
  
  if(varset) { //set variable if there was an assignment
    var->value[check] = out.stk[0];
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
  return searchOperatorArray(buffer);
}


//checks the type of character
int checkType(char a) {
  switch(a) {
    //alpha numeric is 1
    //operators are 2
    //everything else is 0
  case '0' ... '9':
  case '.':
  case 'a' ... 'z':
  case 'A' ... 'Z':
  case '_':
    return 1;

  case '^':
  case '[':
  case ']':
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
  case '\n':
  case ';':
    return 2;

  default:
    return 0;
  }
}


//checks if = and == are in the same spot, relies on strchr and strstr to
//linearlly check from the beginning of the string
int isAssign(char *input) {
>>>>>>> master
  return (strchr(input, '=') == (strstr(input, "=="))) ? 0 : 1;
}
