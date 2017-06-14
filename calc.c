#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "stack.h"
#include "funcs.h"

//06/12/2017


//Shunting-Yard Algorithm
int sya(char *inp, double *ans, vari* var){

  //Variables
  stint out; //output stack
  stchar oper; //operator stack
  int i = 0, j = 0, k = 0, error = 0, cLEP = 0, cREP = 0, length = 0, check = 0, varset = 0, tok = 0;
  char inter[1024], buffer[256], ch,  *str2d;
  //
  
  //reset all the variables
  out.top = 0;
  out.occ = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk));
  memset(buffer, '\0', sizeof(buffer));
  //

  //Error checking
  for(length = 0; inp[length]; length++){
    if(inp[length] == '('){
      cLEP++;
    }
    else if(inp[length] == ')'){
      cREP++;
    }	   
  }
  
  if(cLEP != cREP){
    return error = -3;
  }

  if(strchr("+-/*^(=",inp[length-1])){
    return error = -4;
  }
  //
  
  for(i = 0; inp[i]; ++i){

    ch = inp[i];
    
    switch(ch){      

    case '0' ... '9':
    case '.':
      
      inter[j++] = ch;

      if(inp[i+1] < '0' && inp[i+1] != '.' || inp[i+1] > '9' || !inp[i+1]){

	pushn(strtod(inter, &str2d), &out);
	j = 0;
	memset(inter, '\0', sizeof(inter));
      }
      
      else if((inp[i+1] >= 'a' && inp[i+1] <= 'z') || (inp[i+1] >= 'A' && inp[i+1] <= 'Z')){
	return error = -4;
      }


      tok = 1;
      break;
      
    case '^':

      //      if(strchr("abcdefghi", oper.stk[oper.top])){
      if(oper.stk[oper.top] >= 'a' && oper.stk[oper.top] <= 'z'){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(ch, &oper);
      break;

    case '(':
      tok = 2;
      pushch(ch, &oper);
      break;
      
    case '*':
    case '/':

      while(strchr("*^/abcdefghi", oper.stk[oper.top]) && oper.stk[oper.top] != '\0' && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(ch, &oper);
      break;
      
    case '-':

      if(tok == 2){
	pushn(-1, &out);
	pushch('*', &oper);
	tok = 1;
	break;
      }
      
    case '+':

      while(strchr("+-/*^abcdefghi", oper.stk[oper.top]) && oper.stk[oper.top] != '\0' && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }

      tok = 2;
      pushch(ch, &oper);

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
      
    case 'a' ... 'z':      
    case 'A' ... 'Z':

      buffer[k++] = inp[i];
      
      if(strchr("+-/*()^\n", inp[i+1]) && inp[i+1] != '\n'){

	if(inp[i+1] == '('){
	  buffer[k++] = '(';
	}

	buffer[k] = '\0';
	error = charfind(buffer, &out, &oper, *ans, var, &tok);

	if(error != 0){	  
	  return error;
	}

	memset(buffer, '\0', sizeof(buffer));
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
	  
	  if(++var->count > 256){
	    var->count = 0;
	    printf("\nMaximum number of variables, please clear\n\n");
	  }
	  
	  strcpy(var->name[var->count], buffer);
	  check = var->count;
	}

	k = 0;
	break;
      }//end of if
      
    default: break;
      
    }//end of switch

  }//end of for

  while(out.occ == 1 && oper.occ == 1){
    exec_num(&out, popch(&oper));
  }
  
  if(error == 0){
    printf("\n%lf\n\n", out.stk[0]);
    *ans = out.stk[0];
    
    if(varset == 1){
      var->value[check] = out.stk[0];
    }
    
    return error;
  }
}
//

void errorrep(int error){
  if(error < 0){

    printf("\nError:\n");

    if(error == -1){
      printf("Invalid function or variable name\n\n");
    }

    else if(error == -2){
      printf("No function arguments\n\n");
    }

    else if(error == -3){
      printf("Mismatched parenthesis\n\n");
    }

    else if(error == -4){
      printf("Invalid expression\n\n");
    }
  }
}

int main(int argc, char* argv){
  char* input;
  int error = 0;
  double ans = 0;
  vari var;

  var.count = 0;
  var.occ = 0;

  while(error <= 100){
    input = readline(">>");
    add_history(input);
    
    if( *input == 0){
      continue;
    }
    
    else{
      error = sya(input, &ans, &var);
    }

    errorrep(error);
  }
  return 0;
}
