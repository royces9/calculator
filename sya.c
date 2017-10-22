#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operator.h"
#include "stack.h"
#include "onearg.h"
#include "twoarg.h"
#include "sya.h"

int checkType(char a){
  switch(a){
  case '0' ... '9':
  case '.':
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
  case '\n':
  case ';':
    return 2;

  default:
    return 0;
  }
}

int sya(char *input, double *ans, vari *var){
  numberStack out; //stack for output numbers
  operatorStack oper; //stack for operators
  int i = 0, j = 0, k = 0, error = 0, leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, check = 0, varset = 0, tok = 2;
  char *str2d = NULL;

  removeSpaces(input);
  
  //reset stack variables
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  
  oper.top = 0;
  oper.occ = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));


  //Error checking
  for(length = 0; input[length]; length++){
    if(input[length] == '('){
      leftParenthesisCount++;
    }
    else if(input[length] == ')'){
      rightParenthesisCount++;
    }	   
  }
  
  if(leftParenthesisCount != rightParenthesisCount){
    return error = -3;
  }

  if(strchr("+-/*^(=",input[length-1])){
    return error = -4;
  }

  char bufferLetters[length];
  char bufferOper[length];

  for(i = 0; input[i]; ++i){
    int type = checkType(input[i+1]);
    switch(input[i]){
    case '0' ... '9':
    case '.':
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
      bufferLetters[j++] = input[i];
      if(((type == 2) || (type == 0)) && (input[i+1] != '\n')){
	bufferLetters[j] = '\0';
	if(checkNumbers(bufferLetters)){
	  pushn(strtod(bufferLetters, &str2d), &out);
	  j = 0;
	}
	else{
	  switch(input[i+1]){
	  case '(':
	    bufferLetters[j++] = '(';
	    bufferLetters[j] = '\0';
	    error = findFunction(bufferLetters, &out, &oper, *ans, var, &tok, &i, input);
	    j = 0;
	    break;

	  case '=':
	    bufferLetters[j] = '\0';
	    check = varcheck(var, bufferLetters);
	    varset = 1;

	    if(check >= 0){
	      strcpy(var->name[check], bufferLetters);
	    }
	    else if(check == -1){
	      strcpy(var->name[0], bufferLetters);
	      var->occ = 1;
	      var->count = 0;
	      check = 0;
	    }
	    else if(check == -2){
	      check = ++var->count;
	      strcpy(var->name[check], bufferLetters);
	    }

	    j = 0;
	    break;

	  default:
	    bufferLetters[j] = '\0';
	    error = findFunction(bufferLetters, &out, &oper, *ans, var, &tok, &i, input);
	    j = 0;
	    break;
	  }
	}
      }
      tok = 1;
      break;

    case '^':

    case '*':
    case '/':

    case '-':
    case '+':

    case '(':
    case ')':

    case '=':
    case '>':
    case '<':

    case '!':
      bufferOper[k++] = input[i];
      if((type == 1) || (input[i] == '(') || (input[i] == ')') || (input[i+1] == '(') || (input[i+1] == ')')){
	bufferOper[k] = '\0';
	error = findOperator(bufferOper, &out, &oper, *ans, var, &tok);
	k = 0;
      }

      break;

    case '\n':
    case ';':
      break;

    default:
      return error = -4;
      
    }//end of switch
    if(error < 0){
      return error;
    }
  }//end of for
  
  while((out.occ == 1) && (oper.occ == 1)){
    execNum(&out, popch(&oper));
  }

  *ans = out.stk[0];
  
  if(varset == 1){
    var->value[check] = out.stk[0];
  }

  return error;
}

void errorrep(int error){
  if(error < -1){
    printf("\nError:\n");
    switch(error){
    case -2: printf("Incorrect number of function arguments"); break;
    case -3: printf("Mismatched parenthesis"); break;
    case -4: printf("Invalid expression"); break;
    case -5: printf("Invalid function or variable name"); break;
    case -6: printf("Malloc error"); break;
    default: break;
    }
  printf("\n\n");
  }
}

void removeSpaces(char *input){
  char *a = input;
  char *b = input;
  while(*b != 0){
    *a = *b++;
    if(*a != ' '){
      a++;
    }
  }
  *a = 0;
}

int checkNumbers(char *input){ //check if the input string is a number
  int count = 0;
  for(int i = 0; i  < strlen(input); i++){
    if(input[i] < '0' && input[i] != '.' || input[i] > '9' || !input[i]){
      return 0;
    }
  }
  return 1;
}
