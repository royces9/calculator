#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "strucon.h"
#include "sya.h"
#include "funcs.h"

double vartypeset(vari *var, char inp[]){
  char *str2d;
  int varc = varcheck(var, inp);

  if(varc <= -1){
    return strtod(inp, &str2d);
  }
  else{
    return var->value[varc];
  }
}

double deri(char inp[10][256], vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, point = 0, h = 0;
  vari dvar = *var;
  int varc = 0;

  point = vartypeset(&dvar, inp[2]);
  h = vartypeset(&dvar, inp[3]);
  
  //set up a dummy variable specified by user  
  varc = varcheck(&dvar, inp[1]);

  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  else if(varc == -2){
    varc = ++dvar.count;
  }

  strcpy(dvar.name[varc], inp[1]);

  //sets the dummy variable equal to x+h
  dvar.value[varc] = point + h;
  
  //does f(x+h)
  sya(inp[0], &out, &dvar);

  //sets the dummy variable equal to x-h
  dvar.value[varc] = point - h;

  //does f(x-h)
  sya(inp[0], &inter, &dvar);

  //this is f(x+h) - f(x-h)
  out -= inter;

  //return (f(x)- f(x-h))/(2*h)
  return out/(2*h);
}

double inte(char inp[10][256], vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, step = 0, number = 0, a = 0, b = 0;
  vari dvar = *var;
  int i = 0, varc = 0;

  //get number of steps, and step size
  a = vartypeset(&dvar, inp[2]);
  b = vartypeset(&dvar, inp[3]);
  number = vartypeset(&dvar, inp[4]);
  step = (b-a)/number;

  //set dummy variable
  varc = varcheck(&dvar, inp[1]);
    
  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  else if(varc == -2){
    varc == ++dvar.count;
  }

  //get the value of (f(a)+f(b))/2
  strcpy(dvar.name[varc],inp[1]);
  dvar.value[varc] = a;
  sya(inp[0], &out, &dvar);
  dvar.value[varc] = b;
  sya(inp[0], &inter, &dvar);

  out += inter;
  out /= 2;

  //get the sum of f(a+n*delta)
  for(i = 0; i <= number; i++){
    dvar.value[varc] = step*i+a;
    sya(inp[0], &inter, &dvar);
    out += inter;
    dvar.value[varc]++;
  }

  //returns the integral
  return out * step;    
}

double solve(char inp[10][256], vari *var, int *error){
  char *str2d;
  vari dvar = *var;
  double out = 0, inter = 0, h = 0, test = 0, delta = 0.000001;
  int varc = 0;

  varc = varcheck(&dvar, inp[1]);

  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  else if(varc == -2){
    varc = ++dvar.count;
  }

  strcpy(dvar.name[varc],inp[1]);
  dvar.value[varc] = vartypeset(&dvar, inp[2]);
  h = vartypeset(&dvar, inp[3]);
  test = h + 1;
  while(test > h){
    sya(inp[0],&out, &dvar);
    dvar.value[varc] -= delta;
    sya(inp[0],&inter, &dvar);
    test = (delta*out)/(out-inter);
    dvar.value[varc] -= test;
  }
  return dvar.value[varc];
}


void sep(char inp[], int *start, char sepa[10][256]){
  char *tok, inp2[1024];
  int cLEP = 0, cREP = 0, length = 0, i = 0;

  inp += (*start+1);
  strcpy(inp2,inp);  

  for(length = 0; inp2[length]; length++){
    if(inp2[length] == '('){
      cLEP++;
    }
    else if(inp2[length] == ')'){
      cREP++;
    }

    if(cLEP == cREP){
      break;
    }
  }
 
 *start += (length+1);
  inp2[length+1] = 0;

  tok = strtok(inp2, ",");
  ++tok;

  for(i = 0; tok != NULL; i++){
    strcpy(sepa[i], tok);
    tok = strtok(NULL, ",");
  }

  if(tok == NULL){
    sepa[i-1][strlen(sepa[i-1])-1] = '\0';
  }
  strcpy(sepa[i], "");
}
