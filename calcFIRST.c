#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//zero and nine
#define zero 48
#define nine 57

//four basic operations
#define A 43
#define S 45
#define M 42
#define D 47

//other
#define LEP 40
#define REP 41
#define EXP 94
#define DEC 46


typedef struct{
  double stk[1024];
  int top;
} stint;

typedef struct{
  char stk[1024];
  int top;
} stchar;


////Stack stuff
//numbers
void pushn(double inp, stint* st){
  if(st->stk[0] == 0){
    st->stk[st->top] = inp;
  }
  else{
    st->top++;
    st->stk[st->top] = inp;
  }
  //  st->top++;
}

double popn(stint* st){
  double out = 0;
  /*
  if(st->top < 0){
    return st->stk[st->top];
    }*/
  out = st->stk[st->top];
  st->top--;
  return out;
}
//characters
void pushch(char inp, stchar* st){
  if(st->stk[0] == '\0'){
    st->stk[st->top] = inp;
  }
  else{
  st->top++;
  st->stk[st->top] = inp;
  }
}

char popch(stchar* st){
  char out;
  /*
  if( st->top < 0){
    return st->stk[st->top];
    }*/
  out = st->stk[st->top];
  st->top--;
  return out;
}
//


//Function for operators
double op(double aa, double bb, char O){
  switch(O){
  case '+': return aa + bb;
  case '-': return aa - bb;
  case '*': return aa * bb;
  case '/': return aa / bb;
  case '^': return pow(aa, bb);
  }
}
//


//EXEC function
void exec(stint* ninp, char ch){
  double nA, nB;
  nB = popn(ninp);
  nA = popn(ninp);
  pushn(op(nA, nB, ch), ninp);
}
//


//Shunting-Yard Algorithm
double sya(char inp[]){
  stint out;
  stchar oper;
  int i = 0, j = 0, k = 0;
  char inter[1024], *str2d, operbuff, ch;
  double num = 0;
  
  out.top = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk)); 
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
      inter[j] = ch;
      ++j;
      if((inp[j] < zero && !(inp[j] == DEC) || inp[j] > nine)){
	num = strtod(inter, &str2d);
	pushn(num, &out);
	j = 0;
	memset(inter, 0, sizeof(inter));
      }
      num = 0;
      break;
    case '^':
    case '(':
      pushch(ch, &oper);
      break;
    case '*':
    case '/':
      if(oper.stk[oper.top] == M || oper.stk[oper.top] == D || oper.stk[oper.top] == EXP){
	exec(&out, popch(&oper));
	pushch(ch, &oper);
      }
      else{
	pushch(ch, &oper);
      }
      break;
    case '+':
    case '-':
      if(oper.stk[oper.top] == A || oper.stk[oper.top] == S || oper.stk[oper.top] == M || oper.stk[oper.top] == D || oper.stk[oper.top] == EXP ){
	exec(&out, popch(&oper));
	pushch(ch, &oper);
      }
      else{
	pushch(ch, &oper);
      }
      break;

    case ')':
            while(oper.stk[oper.top] != LEP){
	exec(&out, popch(&oper));
      }
      popch(&oper);
      break;
    default:
      break;
    }

  }
  while(out.top != -1){
    //    printf("out.top %d\n", out.top);
    exec(&out, popch(&oper));
  }
  return out.stk[0];
}
//


int main(){

  char input[1024];
  while(input != "exit"){
    printf(">>");
    scanf("%s", &input);
    printf("\n%lf\n\n", sya(input));
  }
  return 0;
}
