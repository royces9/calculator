#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "strucon.h"
#include "stack.h"
#include "funcs.h"
#include "sya.h"

int sya(char *inp, double *ans, vari *var){
  stint out; //output stack
  stchar oper; //operator stack
  int i = 0, j = 0, k = 0, error = 0, leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, check = 0, varset = 0, tok = 0;
  char *str2d;

  //reset all the variables
  out.top = 0;
  out.occ = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk));

  //Error checking
  for(length = 0; inp[length]; length++){
    if(inp[length] == '('){
      leftParenthesisCount++;
    }
    else if(inp[length] == ')'){
      rightParenthesisCount++;
    }	   
  }
  
  if(leftParenthesisCount != rightParenthesisCount){
    return error = -3;
  }

  if(strchr("+-/*^(=",inp[length-1])){
    return error = -4;
  }

  char *inter = (char *) malloc(length * sizeof(char)), *buffer = (char *) malloc(length * sizeof(char));
  for(i = 0; inp[i]; ++i){
    //    ch = inp[i];

    switch(inp[i]){

    case '0' ... '9':
    case '.':
      inter[j++] = inp[i];

      if(inp[i+1] < '0' && inp[i+1] != '.' || inp[i+1] > '9' || !inp[i+1]){
	inter[j] = '\0';
	pushn(strtod(inter, &str2d), &out);
	j = 0;
      }
      
      else if((inp[i+1] >= 'a' && inp[i+1] <= 'z') || (inp[i+1] >= 'A' && inp[i+1] <= 'Z')){
	return error = -4;
      }

      tok = 1;

      break;
      
    case '^':
      if(oper.stk[oper.top] >= 'a' && oper.stk[oper.top] <= 'z'){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(inp[i], &oper);
      break;

    case '(':
      tok = 2;
      pushch(inp[i], &oper);
      break;
      
    case '*':
    case '/':
      while(strchr("*^/abcdefghi", oper.stk[oper.top]) && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(inp[i], &oper);
      break;
      
    case '-':
      if(tok == 2){
	pushn(-1, &out);
	pushch('*', &oper);
	tok = 1;
	break;
      }
      
    case '+':
      while(strchr("+-/*^abcdefghi", oper.stk[oper.top]) && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }
      tok = 2;
      pushch(inp[i], &oper);

      break;

    case ')':      
      while(oper.stk[oper.top] != '(' && oper.occ == 1){
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

      buffer[k++] = inp[i];

      if(strchr("+-/*()^\n", inp[i+1]) && inp[i+1] != '\n'){

	if(inp[i+1] == '('){
	  buffer[k++] = '(';
	}

	buffer[k] = '\0';

	error = charfind(buffer, &out, &oper, *ans, var, &tok, &i, inp);
	
	if(error != 0){
	  return error;
	}
	k = 0;
      }
      
      else if('=' ==  inp[i+1] && inp[i+1] != '\n'){

	check = varcheck(var, buffer);
	varset = 1;
	
	if(check >= 0){
	  buffer[k] = '\0';
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
	k = 0;
      }//end of if
      break;      

    default: break;
      
    }//end of switch
    if(error != 0){
      return error;
    }
  }//end of for

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
    case -2: printf("Deficient number of function arguments\n\n"); break;
    case -3: printf("Mismatched parenthesis\n\n"); break;
    case -4: printf("Invalid expression\n\n"); break;
    case -5: printf("Invalid function or variable name\n\n"); break;
    default: break;
    }
  }
}
