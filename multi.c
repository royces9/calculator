#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "strucon.h"
#include "sya.h"
#include "funcs.h"

double vartypeset(vari *var, char input[]){
  char *str2d;
  int varc = varcheck(var, input);

  if(varc <= -1){
    return strtod(input, &str2d);
  }
  else{
    return var->value[varc];
  }
}
int numberOfArgs(char **input){
  int i = 0;
  for(i = 0; strcmp(input[i], "\0"); i++);
  return i;
}

double deri(char **input, vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, point = 0, h = 0;
  vari dvar = *var;
  int varc = 0;

  //check the number of inputs is correct
  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }

  point = vartypeset(&dvar, input[2]);
  h = vartypeset(&dvar, input[3]);
  
  //set up a dummy variable specified by user  
  varc = varcheck(&dvar, input[1]);

  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  else if(varc == -2){
    varc = ++dvar.count;
  }

  strcpy(dvar.name[varc], input[1]);

  //sets the dummy variable equal to x+h
  dvar.value[varc] = point + h;
  
  //does f(x+h)
  sya(input[0], &out, &dvar);

  //sets the dummy variable equal to x-h
  dvar.value[varc] = point - h;

  //does f(x-h)
  sya(input[0], &inter, &dvar);

  //this is f(x+h) - f(x-h)
  out -= inter;

  //return (f(x)- f(x-h))/(2*h)
  return out/(2*h);
}

double inte(char **input, vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, step = 0, number = 0, a = 0, b = 0;
  vari dvar = *var;
  int varc = 0;

  if(numberOfArgs(input) != 5){
    *error = -2;
    return 0;
  }


  //get number of steps, and step size
  a = vartypeset(&dvar, input[2]);
  b = vartypeset(&dvar, input[3]);
  number = vartypeset(&dvar, input[4]);
  step = (b-a)/number;

  //set dummy variable
  varc = varcheck(&dvar, input[1]);
    
  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  else if(varc == -2){
    varc == ++dvar.count;
  }

  //get the value of (f(a)+f(b))/2
  strcpy(dvar.name[varc],input[1]);
  dvar.value[varc] = a;
  sya(input[0], &out, &dvar);
  dvar.value[varc] = b;
  sya(input[0], &inter, &dvar);

  out += inter;
  out /= 2;

  //get the sum of f(a+n*delta)
  for(int i = 0; i <= number; i++){
    dvar.value[varc] = step*i+a;
    sya(input[0], &inter, &dvar);
    out += inter;
    dvar.value[varc]++;
  }

  //returns the integral
  return out * step;    
}

double solve(char **input, vari *var, int *error){
  char *str2d;
  vari dvar = *var;
  double out = 0, inter = 0, h = 0, test = 0, delta = 0.000001;
  int varc = 0;

  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }
    
  varc = varcheck(&dvar, input[1]);

  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  else if(varc == -2){
    varc = ++dvar.count;
  }

  strcpy(dvar.name[varc],input[1]);
  dvar.value[varc] = vartypeset(&dvar, input[2]);
  h = vartypeset(&dvar, input[3]);
  test = h + 1;
  while(fabs(test) > h){
    sya(input[0],&out, &dvar);
    dvar.value[varc] -= delta;
    sya(input[0],&inter, &dvar);
    test = (delta*out)/(out-inter);
    dvar.value[varc] -= test;
  }
  return dvar.value[varc];
}


char **separateString(char input[], int *start){
  char *tok;
  int leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, commaCount = 0, i = 0;  

  input += (*start+1);

  for(length = 0; input[length]; length++){
    if(input[length] == '('){
      leftParenthesisCount++;
    }
    else if(input[length] == ')'){
      rightParenthesisCount++;
    }
    if(input[length] == ','){
      commaCount++;
    }
    if(leftParenthesisCount == rightParenthesisCount){
      break;
    }
  }

  char *input2 = malloc(length * sizeof(*input2));
  strcpy(input2,input);
  
  //allocate double array output
  char **separatedString = malloc((commaCount + 2) * sizeof(*separatedString));

  for(int j = 0; j < (commaCount + 2); j++){
    separatedString[j] = malloc(length * sizeof(**separatedString));
  }

  *start += (length+1);
  input2[length+1] = 0;

  tok = strtok(input2, ",");
  ++tok;

  for(i = 0; tok != NULL; i++){
    strcpy(separatedString[i], tok);
    tok = strtok(NULL, ",");
  }

  separatedString[i-1][strlen(separatedString[i-1])-1] = '\0';

  strcpy(separatedString[i], "");
  free(input2);
  
  return separatedString;
}
