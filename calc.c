#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

//06/10/2017


//constants
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442

#define E 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260595630738132328627943490763233829880753195251019011573

# define NF 16 //Number of functions+constants

#define SIN 'a'
#define COS 'b'
#define TAN 'c'
#define LN 'd'
#define LOG 'e'
#define SQRT 'f'
#define ASIN 'g'
#define ACOS 'h'
#define ATAN 'i'

const char FUNCTIONS[NF][10] = {"pi", "e", "ans", "sin(", "cos(", "tan(", "ln(", "log(", "sqrt(", "asin(", "acos(", "atan(", "quit", "clear", "list", "help"};

//STRUCTS
typedef struct{
  double stk[1024]; //stack
  int top; //The index for the top of the stack
  int occ; //is 1 if the stack is occupied, 0 if it's empty
} stint;

typedef struct{
  char stk[1024]; //stack
  int top; //The index for the top of the stack
  int occ;
} stchar;

typedef struct{
  char name[256][256];
  double value[256];
  int occ;
  int count;
} vari;
//


////Stack stuff
//numbers
void pushn(double inp, stint* st){

  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }
  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

double popn(stint* st){

  double out;
  if(st->occ == 1){
    out = st->stk[st->top--];
    
    if(st->top == -1){
      st->occ = 0;
      st->top = 0;
    }    
    return out;
  }
  else{
    return 0;
  }
}
//characters
void pushch(char inp, stchar* st){

  if(st->occ == 1){
    st->stk[++st->top] = inp;
  }
  
  else{
    st->stk[0] = inp;
    st->occ= 1;
  }
}

char popch(stchar* st){

  char out;
  if(st->occ == 1){
    out = st->stk[st->top--];
    
    if(st->top == -1){
      st->stk[0] == '\0';
      st->occ = 0;
      st->top = 0;
    }
    
    return out;
  }
  else{
    return 0;
  }
}
//

//Function for operators
double op(double a, double b, char o){

  switch(o){
  case '+': return a + b;
  case '-': return a - b;
  case '*': return a * b;
  case '/': return a / b;
  case '^': return pow(a, b);
  }
}

double ops(double a, char o){

  switch(o){
  case SIN: return sin(a);
  case COS: return cos(a);
  case TAN: return tan(a);
  case LN: return log(a);
  case LOG: return log10(a);
  case SQRT: return sqrt(a);
  case ASIN: return asin(a);
  case ACOS: return acos(a);
  case ATAN: return atan(a);
  }
}
//

//execute function
void exec_num(stint* num, char ch){

  double a, b;

  switch(ch){

  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
    b = popn(num);
    a = popn(num);
    pushn(op(a, b, ch), num);
    break;
      
  case 'a' ... 'z':
  case 'A' ... 'Z':
    a = popn(num);
    pushn(ops(a, ch), num);
    break;
    
  default:
    break;
  }
}

//

int funcfind(char buffer[]){

  for(int i = 0; i < NF; i++){
    if(!strcmp(FUNCTIONS[i], buffer)){
      return i;
    }
  }
  return NF;
}

//for alphabet

int charfind(char buffer[], stint* num, stchar* ch, double ans, vari* var, int* tok){

  int i = funcfind(buffer);

  switch(i){
  case 0:
    pushn(PI, num);
    *tok = 1;
    return 0;

  case 1:
    pushn(E, num);
    *tok = 1;
    return 0;

  case 2:
    pushn(ans, num);
    *tok = 1;
    return 0;

  case 3:
    pushch(SIN, ch);
    *tok = 2;
    return 0;

  case 4:
    pushch(COS, ch);
    *tok = 2;
    return 0;

  case 5:
    pushch(TAN, ch);
    *tok = 2;
    return 0;

  case 6:
    pushch(LN, ch);
    *tok = 2;
    return 0;

  case 7:
    pushch(LOG, ch);
    *tok = 2;
    return 0;

  case 8:
    pushch(SQRT, ch);
    *tok = 2;
    return 0;
    
  case 9:
    pushch(ASIN, ch);
    *tok = 2;
    return 0;
    
  case 10:
    pushch(ACOS, ch);
    *tok = 2;
    return 0;
    
  case 11:
    pushch(ATAN, ch);
    *tok = 2;
    return 0;
    
  case NF - 4: //quit
    return 2;
    
  case NF - 3: //clear
    memset(var->name, '\0', sizeof(var->name));
    memset(var->value, 0, sizeof(var->value));
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n\n");
    return 1;
    
  case NF - 2: //list
    if(var->occ != 0){
      printf("\nVariable List:\n");
      for(int j = 0; j <= var->count; j++){
	printf("%s = %lf\n", var->name[j], var->value[j]);
      }
      printf("\n");
    }
    else{
      printf("\nNo variables set\n\n");
    }
    return 1;

  case NF - 1: //help
    printf("quit - quit program\nlist - list variables\nclear - clear variables\n\n");
    return 1;
    
  case NF: //variables
    for(i = 0; i <= var->count; i++){
      if(!strcmp(buffer, var->name[i])){
	pushn(var->value[i], num);
	*tok = 1;
	return 0;	
      }
    }
    break;
    
  default:
    break;

  }//end of switch

return -1;
}

int varcheck(vari* list, char inp[]){

  int i = 0;

  if(list->occ == 0){
    return -1;
  }

  for(i = 0; i<=list->count; i++){
    if(!strcmp(inp, list->name[i])){
      return i;
    }    
  }
  
  return -2;
}

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

      if(strchr("~!@#$%", oper.stk[oper.top])){
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

      while(strchr("*^/~!@#$%", oper.stk[oper.top]) && oper.stk[oper.top] != '\0' && oper.occ == 1){
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

      while(strchr("+-/*^~!@#$%", oper.stk[oper.top]) && oper.stk[oper.top] != '\0' && oper.occ == 1){
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

  while(error <= 1){
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
