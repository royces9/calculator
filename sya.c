#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "funcs.h"
#include "sya.h"

int sya(char *input, double *ans, vari *var){
  numberStack out; //stack for output numbers
  operatorStack oper; //stack for operators
  int i = 0, j = 0, error = 0, leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, check = 0, varset = 0, tok = 0;
  char *str2d;

  //reset all the variables
  out.top = 0;
  out.occ = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk));

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

  char *buffer = malloc(length * sizeof(*buffer));
  __MALLOC_CHECK(buffer, error);

  for(i = 0; input[i]; ++i){
    switch(input[i]){

    case '0' ... '9':
    case '.':
    case 'a' ... 'z':
    case 'A' ... 'Z':
      buffer[j++] = input[i];

      if(strchr("=+-/*()^\n", input[i+1]) && input[i+1] != '\n'){
	buffer[j] = '\0';
	if(checkNumbers(buffer)){
	  pushn(strtod(buffer, &str2d), &out);
	  j = 0;
	}
	else{
	  switch(input[i+1]){
	  case '(':
	    buffer[j++] = '(';
	    buffer[j] = '\0';
	    error = charfind(buffer, &out, &oper, *ans, var, &tok, &i, input);
	    j = 0;
	    break;

	  case '=':
	    buffer[j] = '\0';
	    check = varcheck(var, buffer);
	    varset = 1;

	    if(check >= 0){
	      strcpy(var->name[check], buffer);
	    }	

	    else if(check == -1){
	      strcpy(var->name[0], buffer);
	      var->occ = 1;
	      var->count = 0;
	      check = 0;
	    }
	    else if(check == -2){
	      check = ++var->count;
	      strcpy(var->name[check], buffer);
	    }

	    j = 0;
	    break;

	  default:
	    buffer[j] = '\0';
	    error = charfind(buffer, &out, &oper, *ans, var, &tok, &i, input);
	    j = 0;
	    break;
	  }
	}
      }
      tok = 1;
      break;

    case '^':
      if(oper.stk[oper.top].operator >= 'a' && oper.stk[oper.top].operator <= 'z'){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(setOpStack(input[i], 2), &oper);
      break;

    case '(':
      tok = 2;
      pushch(setOpStack(input[i], 2), &oper);
      break;
      
    case '*':
    case '/':
      while(strchr("*^/abcdefghi", oper.stk[oper.top].operator) && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(setOpStack(input[i], 2), &oper);
      break;
      
    case '-':
      if(tok == 2){
	pushn(-1, &out);
	pushch(setOpStack('*', 2), &oper);
	tok = 1;
	break;
      }
      
    case '+':
      while(strchr("+-/*^abcdefghi", oper.stk[oper.top].operator) && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }
      tok = 2;
      pushch(setOpStack(input[i], 2), &oper);

      break;

    case ')':      
      while(oper.stk[oper.top].operator != '(' && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }
      tok = 1;
      popch(&oper);
      break;
      
    case '=':
      if(varset == 0){
	return error = -4;
      }
      break;

    case '!':
      pushn(factorial(popn(&out), &error), &out);
      break;

    default: break;
      
    }//end of switch
    if(error != 0){
      return error;
    }
  }//end of for
  
  free(buffer);

  while(out.occ == 1 && oper.occ == 1){
    exec_num(&out, popch(&oper));
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
    case -2: printf("Incorrect number of function arguments\n\n"); break;
    case -3: printf("Mismatched parenthesis\n\n"); break;
    case -4: printf("Invalid expression\n\n"); break;
    case -5: printf("Invalid function or variable name\n\n"); break;
    case -6: printf("Malloc error.\n\n"); break;
    default: break;
    }
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

int checkNumbers(char *input){
  for(int i = 0; i  < strlen(input); i++){
    if(input[i] < '0' && input[i] != '.' || input[i] > '9' || !input[i]){
      return 0;
    }
  }
  return 1;
}
