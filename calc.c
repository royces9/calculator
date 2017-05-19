#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//05/16/2017


//constants
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442

#define E 2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260595630738132328627943490763233829880753195251019011573


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
double op(double a, double b, char O){
  switch(O){
  case '+': return a + b;
  case '-': return a - b;
  case '*': return a * b;
  case '/': return a / b;
  case '^': return pow(a, b);
  }
}

double ops(double a, char O){
  switch(O){
  case '~': return sin(a);
  case '!': return cos(a);
  case '@': return tan(a);
  case '#': return log(a);
  case '$': return log10(a);
  case '%': return sqrt(a);
  }
}
//

//execute function
void exec_num(stint* num, char ch){
  double nA, nB;
  switch(ch){
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
    nB = popn(num);
    nA = popn(num);
    pushn(op(nA, nB, ch), num);
    break;
    
  case '~':
  case '!':
  case '@':
  case '#':
  case '$':
  case '%':
    nA = popn(num);
    pushn(ops(nA, ch), num);
    break;
    
  default:
    break;
  }
}

//

//for alphabet
int charfind(char buffer[], stint* num, stchar* ch, double ans, vari* var){
  int i = 0;

  if (!strcmp(buffer, "pi")){
    pushn(PI, num);
    return 0;
  }
  
  else if(!strcmp(buffer, "e")){
    pushn(E, num);
    return 0;
  }
  
  else if(!strcmp(buffer, "ans")){
    pushn(ans, num);
    return 0;
  }
  
  else if(!strcmp(buffer, "sin")){
    pushch('~', ch);
    return 0;
  }
  
  else if(!strcmp(buffer, "cos")){
    pushch('!', ch);
    return 0;
  }
  
  else if(!strcmp(buffer, "tan")){
    pushch('@', ch);
    return 0;
  }
  
  else if(!strcmp(buffer, "ln")){
    pushch('#', ch);
    return 0;
  }
  
  else if(!strcmp(buffer, "log")){
    pushch('$', ch);
    return 0;
  }
  
  else if(!strcmp(buffer, "sqrt")){
    pushch('%', ch);
    return 0;
  }
  
  else if(!strcmp(buffer, "clear")){
    memset(var->name, '\0', sizeof(var->name));
    memset(var->value, 0, sizeof(var->value));
    var->occ = 0;
    var->count = 0;
    printf("\nAll variables cleared\n");
    return 0;
  }
  
  else{
    for(i = 0; i <= var->count; i++){
      if(!strcmp(buffer, var->name[i])){
	pushn(var->value[i], num);
	return 0;
      }
    }
  }
  return -2;
}

int varcheck(vari* list, char inp[]){
  int i = 0;
  
  for(i = 0; i<=list->count; i++){
    if(list->occ == 0){
	return -1;
    }
    
    else if(!strcmp(inp, list->name[i])){
      printf("%d\n", i);
      return i;
    }
    
  }
  
  return -2;
}

//Shunting-Yard Algorithm
double sya(char inp[], double *ans, vari* var){

  //Variables
  stint out; //output stack
  stchar oper; //operator stack
  int i = 0, j = 0, k = 0, error = 0, cLEP = 0, cREP = 0, length = 0, check = 0, varset = 0;
  char inter[1024], buffer[256], ch,  *str2d;
  double num = 0;
  //

  for(length = 0; inp[length]; length++){}
  
  //reset all the variables
  out.top = 0;
  out.occ = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk));
  memset(buffer, '\0', sizeof(buffer));
  //
  
  for(i = 0; inp[i]; ++i){

    ch = inp[i];
    
    switch(ch){      
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
      
      inter[j] = ch;
      ++j;
      
      if(inp[i+1] < '0' && inp[i+1] != '.' || inp[i+1] > '9' || !inp[i+1]){
	num = strtod(inter, &str2d);
	pushn(num, &out);
	j = 0;
	memset(inter, '\0', sizeof(inter));
	num = 0;
      }
      
      break;
      
    case '^':
      if(strchr("~!@#$%", oper.stk[oper.top])){
	exec_num(&out, popch(&oper));
      }
      
      pushch(ch, &oper);
      break;

    case '(':
      cLEP++;
      pushch(ch, &oper);
      break;
      
    case '*':
    case '/':
      while(strchr("*^/~!@#$%", oper.stk[oper.top]) && oper.stk[oper.top] != '\0' && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }
      pushch(ch, &oper);
      break;
      
    case '-':
      if(inp[i-1] < '0' || inp[i-1] > '9'){
	pushn(-1, &out);
	pushch('*', &oper);
	break;
      }
      
    case '+':
      while(strchr("+-/*^~!@#$%", oper.stk[oper.top]) && oper.stk[oper.top] != '\0' && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }
    
      pushch(ch, &oper);
      break;

    case ')':
      cREP++;
      
      while(oper.stk[oper.top] != '(' && oper.occ == 1){
	exec_num(&out, popch(&oper));
      }
      
      popch(&oper);
      break;
      
      /*    case '!':
      pushch (ch ,&oper);
      exec_num(&out, popch(&oper));
      break;*/
      
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
      
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      
      buffer[k++] = inp[i];
      
      if(strchr("+-/*()^\n", inp[i+1]) && inp[i+1] != '\0'){
	buffer[k] = '\0';
	error = charfind(buffer, &out, &oper, *ans, var);
	memset(buffer, '\0', sizeof(buffer));
	k = 0;
      }
      
      else if(strchr("=", inp[i+1]) && inp[i+1] != '\0'){
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
	  var->count++;
	  
	  if(var->count > 256){
	    var->count = 0;
	  }
	  
	  strcpy(var->name[var->count], buffer);
	  check = var->count;
	}
	k = 0;
	break;
      }//end of if
      
    default: break;
    }//end of switch
    if(i == length-1){
      if(cLEP != cREP){
	error = -1;
      }
    }
    
    if(error != 0){
      printf("\nError:\n");
      break;
    }

  }//end of for

  while(out.top > -1 && out.occ == 1 && oper.occ == 1){
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
  else if(error == -1){
    printf("Mismatched parenthesis\n\n");   
  }
  else if(error == -2){
    printf("Invalid function or variable name\n\n");
    return error;
  }
  
  else if(error == -3){
    printf("No function arguments\n\n");
    return error;
  }
}
//


int main(){
  char input[1024];
  double ans = 0;
  vari var;
  var.count = 0;
  var.occ = 0;
  
  while(input != "q"){
    printf(">>"); // separator to know when to put input    
    fgets(input, 1024, stdin);
    
    if((input[0] == 'q' && input[1] == '\n')){ //break when input is 'q'
      break;
    }
    
    else if(input[0] == '\n'){
      continue;
    }
    
    else{
      sya(input, &ans, &var);
    }
  }
  return 0;
}
