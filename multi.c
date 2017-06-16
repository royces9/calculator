#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "strucon.h"
#include "sya.h"
#include "funcs.h"

double deri(char inp[10][1024], vari *var){
  char *str2d;
  double out = 0, inter = 0, point = strtod(inp[2], &str2d), h = strtod(inp[3], &str2d);
  vari dvar = *var;
  int varc = varcheck(&dvar, inp[1]);;

  //set up a dummy variable specified by user
  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  strcpy(dvar.name[varc], inp[1]);
  dvar.value[varc] = point;
  
  //does f(x)
  sya(inp[0], &out, &dvar);

  //sets the dummy variable equal to x-h
  dvar.value[varc] = point - h;

  //does f(x-h)
  sya(inp[0], &inter, &dvar);

  //this is f(x) - f(x-h)
  out -= inter;

  //return (f(x)- f(x-h))/h
  return out/h;
}

double inte(char inp[10][1024], vari *var){
  double out = 0, inter = 0, step = 0, number = 0, a = 0, b = 0;
  vari dvar = *var;
  char *str2d;
  int i = 0, varc = 0;

  //get number of steps, and step size
  number = strtod(inp[4], &str2d);
  a = strtod(inp[2], &str2d);
  b = strtod(inp[3], &str2d);
  step = (b-a)/number;

  varc = varcheck(&dvar, inp[1]);
  if(varc == -1){
    varc = 0;
    dvar.occ = 1;
  }
  strcpy(dvar.name[varc],inp[1]);
  dvar.value[varc] = a;
  sya(inp[0], &out, &dvar);
  dvar.value[varc] = b;
  sya(inp[0], &inter, &dvar);

  out += inter;
  out /= 2;
  
  for(i = 0; i <= number; i++){
    dvar.value[varc] = step*i+a;    
    sya(inp[0], &inter, &dvar);
    out += inter;
    dvar.value[varc]++;
  }
  return out * step;    
}

void sep(char inp[], int *start, char sepa[10][1024]){

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
  tok += 1;
  for(i = 0; tok != NULL; i++){
    strcpy(sepa[i], tok);
    tok = strtok(NULL, ",");
  }
  if(tok == NULL){
    length=strlen(sepa[i-1]);
    sepa[i-1][length-1] = '\0';
  }
  strcpy(sepa[i], "");
}


double multifunc(int type, char inp[], int *start, vari *var){
  char sepa[10][1024];

  switch(type){
  case 16:
    sep(inp, start, sepa);
    return deri(sepa, var); 
  case 17:
    sep(inp, start, sepa);
    return inte(sepa, var);
  default: break;
  }
}


