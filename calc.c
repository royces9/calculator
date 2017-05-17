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
void exec_num(stint* ninp, char ch){
  double nA, nB;
  switch(ch){
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
    nB = popn(ninp);
    nA = popn(ninp);
    pushn(op(nA, nB, ch), ninp);
    break;
    
  case '~':
  case '!':
  case '@':
  case '#':
  case '$':
  case '%':
    nA = popn(ninp);
    pushn(ops(nA, ch), ninp);
    break;
    
  default:
    break;
  }
}

//


//Shunting-Yard Algorithm
double sya(char inp[], double *ans){

  //Variables
  stint out; //output stack
  stchar oper; //operator stack
  int i = 0, j = 0, error = 0, cLEP = 0, cREP = 0, length = 0;
  char inter[1024], *str2d, ch;
  double num = 0;
  //


  //  for(length = 0; inp[length]; ++length){}
  
  //reset all the variables
  out.top = 0;
  out.occ = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk));
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
     	//printf("test2\n");
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
      
    case 'a': break;
    case 'b': break;
    case 'c': break;
    case 'd': break;
    case 'e':
      if(strchr("+-*/()^\n", inp[i+1]) && strchr("+-*/()^", inp[i-1])){
	pushn(E, &out);
      }
      break;
    case 'f': break;
    case 'g':
      if(inp[i+1] == '(' && inp[i-1] == 'o' && inp[i-2] =='l'){
	pushch('$', &oper);
      }
      break;
    case 'h': break;
    case 'i':
      if(strchr("+-*/^()\n", inp[i+1]) && inp[i-1] == 'p'){
	pushn(PI, &out);
      }	
      break;
    case 'j': break;
    case 'k': break;
    case 'l': break;
    case 'm': break;
    case 'n':
      if(inp[i+1] == '(' && inp[i-1] == 'i' && inp[i-2] == 's'){
	pushch('~', &oper);
      }
      if(inp[i+1] == '(' && inp[i-1] == 'a' && inp[i-2] == 't'){
	pushch('@', &oper);
      }
      if(inp[i+1] == '(' && inp[i-1] == 'l'){
	pushch('#', &oper);
      }
      break;
    case 'o': break;
    case 'p': break;
    case 'q': break;
    case 'r': break;
    case 's':
      if(inp[i+1] == '(' && inp[i-1] == 'o' && inp[i-2] == 'c'){
	pushch('!', &oper);
      }
      if(strchr("+-/*()^\n", inp[i+1]) && inp[i-1] == 'n' && inp[i-2] == 'a'){
	pushn(*ans, &out);
      }
      break;
    case 't':
      if(inp[i+1] == '(' && inp[i-1] == 'r' && inp[i-2] == 'q' && inp[i-3] == 's'){
	pushch('%', &oper);
      }
      break;
    case 'u': break;
    case 'v': break;
    case 'w': break;
    case 'x': break;
    case 'y': break;
    case 'z': break;

    default:
      break;
    }
    if(error == -1){
      printf("Error");
      return error;
    }
  }

  while(out.top > -1 && out.occ == 1 && oper.occ == 1){
    exec_num(&out, popch(&oper));
  }
  if(cLEP != cREP){
    error = -1;
    printf("\nMismatched parenthesis\n\n");
    return error;
  }
  if(error == 0){
    printf("\n%lf\n\n", out.stk[0]);
    *ans = out.stk[0];
    return error;
  }
}
//


int main(){
  char input[1024];
  double ans = 0;
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
      sya(input, &ans);
    }
  }
  return 0;
}
