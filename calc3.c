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

//letters
#define a 97
#define b 98
#define c 99
#define d 100
#define e 101
#define f 102
#define g 103
#define h 104
#define i 105
#define j 106
#define k 107
#define l 108
#define m 109
#define n 110
#define o 111
#define p 112
#define q 113
#define r 114
#define s 115
#define t 116
#define u 117
#define v 118
#define w 119
#define x 120
#define y 121
#define z 122


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
  if(st->stk[0] == 0 && st->occ == 0){ //if the first character in the stack is 0, AND the stack is empty, do this
    st->top = 0;
    st->stk[st->top] = inp;
    st->occ = 1; //stack is now occupied, set occ to 1
  }
  else{
    st->top++;
    st->stk[st->top] = inp;
    st->occ = 1; 
  }
}

double popn(stint* st){
  double out = 0;
  if(st->top <= 0 && st->occ == 1){
    st->occ = 0;
    st->top = -1;
    return st->stk[st->top];
  }
  else{
  out = st->stk[st->top];
  st->top--;
  }
  return out;
}
//characters
void pushch(char inp, stchar* st){
  if(st->stk[0] == '\0'){ //if the stack begins with null, that means it's empty, and the top is 0
    st->top = 0;
    st->stk[st->top] = inp;
    st->occ = 1;
  }
  else{
  st->top++;
  st->stk[st->top] = inp;
  st->occ = 1;
  }
}

char popch(stchar* st){
  char out;
  if(st->top <=0 && st->occ ==1){
    st->top = 0;
    out = st->stk[st->top];
    st->occ = 0;
  }
  else{
    out = st->stk[st->top];
    st->top--;
  }
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

double ops(double aa, char O){
  switch(O){
  case '~' : return sin(aa);
  case '!' : return cos(aa);
  case '@' : return tan(aa);
  case '#' : return log(aa);
  case '$' : return log10(aa);
  }
}
//


//EXEC function
void exec(stint* ninp, char ch){
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
    nA = popn(ninp);
    pushn(ops(nA, ch), ninp);
    break;
  default:
    break;
  }
}

void execs(stint* ninp, char ch){
  double nA;
  nA = popn(ninp);
  pushn(ops(nA, ch), ninp);
}
//


//Shunting-Yard Algorithm
double sya(char inp[]){

  //Variables
  stint out; //output stack
  stchar oper; //operator stack
  int ii = 0, jj = 0;
  char inter[1024], *str2d, ch, bch, bbch, bbbch, ach;
  double num = 0;
  //

  //reset all the variables
  out.top = 0;
  out.occ = 0;
  oper.top = 0;
  memset(oper.stk, '\0', sizeof(oper.stk));
  memset(out.stk, 0, sizeof(out.stk));
  //
  
  for(ii = 0; inp[ii]; ++ii){

    bbbch = inp[ii-1];
    bbch = inp[ii-1];
    bch = inp[ii-1];
    ch = inp[ii];
    ach = inp[ii+1];

    switch(ch){;;
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
      inter[jj] = ch;
      ++jj;

      if(ach < zero && ach != DEC || ach > nine || !ach){
	num = strtod(inter, &str2d);
	pushn(num, &out);
	jj = 0;
	memset(inter, '\0', sizeof(inter));
	num = 0;
      }

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
      
    case '-':
      if(bch < zero || bch > nine){
	pushn(-1, &out);
	pushch('*', &oper);
	break;
      }
    case '+':
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
      
    case '~':
      execs(&out, popch(&oper));

    case 'a': break;
    case 'b': break;
    case 'c': break;
    case 'd': break;
    case 'e':
      if((ach == A || ach == S || ach == M || ach == D || ach == LEP || ach == REP || ach == EXP || ach == 0) && (bch == A || bch == S || bch == M || bch == D || bch == LEP || bch == REP || bch == EXP || bch == 0)){
	pushn(E, &out);
      }
      break;
    case 'f': break;
    case 'g':
      if(ach == LEP && bch == o && bbch ==l){
	pushch('$', &oper);
      }
      break;
    case 'h': break;
    case 'i': break;
    case 'j': break;
    case 'k': break;
    case 'l': break;
    case 'm': break;
    case 'n':
      if(ach == LEP && bch == i && bbch == s){
	pushch('~', &oper);
      }
      if(ach == LEP && bch == a && bbch == t){
	pushch('@', &oper);
      }
      if(ach == LEP && bch == l){
	pushch('#', &oper);
      }
      break;
    case 'o': break;
    case 'p': break;
    case 'q': break;
    case 'r': break;
    case 's':
      if(ach == LEP && bch == o && bbch == c){
	pushch('!', &oper);
      }
      break;
    case 't': break;
    case 'u': break;
    case 'v': break;
    case 'w': break;
    case 'x': break;
    case 'y': break;
    case 'z': break;

    default:
      break;
    }
  }
  while(out.top != -1){
    exec(&out, popch(&oper));
  }
  return out.stk[0];
}
//


int main(){
  char input[1024]; //input string
  while(input != "q"){
    printf(">>"); // separator to know when to put input
    scanf("%s", &input);
    if(input[0]-113 == 0){ //break when input is 'q'
      break;
    }
    else{
    printf("\n%lf\n\n", sya(input));
    }
  }
  return 0;
}
