#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strucon.h"
#include "funcs.h"
#include "stack.h"
#include "sya.h"

int sya(char *input, double *ans, vari *var){
  numberStack out; //number stack
  operatorStack oper;
  int i = 0, j = 0, k = 0, error = 0, leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, check = 0, varset = 0, tok = 0;
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

  char *numberBuffer = malloc(length * sizeof(*numberBuffer)), *letterBuffer = malloc(length * sizeof(*letterBuffer));
  memset(letterBuffer, '\0', sizeof(letterBuffer));

  for(i = 0; input[i]; ++i){

    switch(input[i]){

    case '0' ... '9':
    case '.':
      numberBuffer[j++] = input[i];

      if(input[i+1] < '0' && input[i+1] != '.' || input[i+1] > '9' || !input[i+1]){
	numberBuffer[j] = '\0';
	pushn(strtod(numberBuffer, &str2d), &out);
	j = 0;
      }
      
      else if((input[i+1] >= 'a' && input[i+1] <= 'z') || (input[i+1] >= 'A' && input[i+1] <= 'Z')){
	return error = -4;
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
      
    case 'a' ... 'z':      
    case 'A' ... 'Z':

      letterBuffer[k++] = input[i];

      if(strchr("+-/*()^\n", input[i+1]) && input[i+1] != '\n'){

	if(input[i+1] == '('){
	  letterBuffer[k++] = '(';
	}

	letterBuffer[k] = '\0';
	error = charfind(letterBuffer, &out, &oper, *ans, var, &tok, &i, input);
	
	if(error != 0){
	  return error;
	}
	k = 0;
      }
      
      else if('=' ==  input[i+1] && input[i+1] != '\n'){

	check = varcheck(var, letterBuffer);
	varset = 1;
	
	if(check >= 0){
	  letterBuffer[k] = '\0';
	  strcpy(var->name[check], letterBuffer);
	}	

	else if(check == -1){
	  strcpy(var->name[0], letterBuffer);
	  var->occ = 1;
	  var->count = 0;
	  check = 0;
	}

	else if(check == -2){
	  check = ++var->count;
	  strcpy(var->name[check], letterBuffer);
	}
	
	k = 0;
      }//end of if
      break;      

    default: break;
      
    }//end of switch
    if(error != 0){
      return error;
    }
  }//end of for
  
  free(numberBuffer);
  free(letterBuffer);

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
