#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "strucon.h"
#include "sya.h"
#include "funcs.h"

char* mystrcat(char *dest, char *src){
  while (*dest) dest++; 
  while (*dest++ = *src++);
  return --dest;
}

void set_var(char set[], char name[], char value[]){
  strcpy(set, name);
  strcat(set, "=");
  strcat(set, value);
}

double deri(char inp[10][1024], vari *var){
  double out = 0, inter = 0;
  vari dvar = *var;
  char setvar[1024], *setvarp, *inpp;

  // set up pointers for the rest
  setvarp = &setvar[0];
  inpp = &inp[0][0];

  //set up a dummy variable specified by user
  set_var(setvar, inp[1], inp[2]);
  sya(setvarp, &out, &dvar);

  //does f(x)/h
  strcat(inp[0], "/");
  strcat(inp[0], inp[3]);  
  sya(inpp, &out, &dvar);

  //sets the dummy variable equal to x-h
  strcat(inp[2], "-");
  strcat(inp[2], inp[3]);
  set_var(setvar, inp[1], inp[2]);
  sya(setvarp, &inter, &dvar);

  //does f(x-h)/h
  sya(inpp, &inter, &dvar);

  //this is (f(x)/h) - (f(x-h)/h)
  out -= inter;

  return out;
}

double inte(char inp[10][1024], vari *var){
  double out = 0, inter = 0, step= 0, number = 0;
  vari dvar = *var;
  char setvar[1024], setvar2[1024], *setvarp, *inpp, *setvarp2, *str2d;
  int i = 0;
  //set up pointers
  setvarp2 = &setvar2[0];
  setvarp = &setvar[0];
  inpp = &inp[0][0];

  //get number of steps, and step size
  number = strtod(inp[4], &str2d);
  step = strtod(inp[3], &str2d);
  step -= strtod(inp[2], &str2d);
  step /= number;

  set_var(setvar, inp[1], inp[2]);
  sya(setvarp, &out, &dvar);
  
  set_var(setvar, inp[1], inp[3]);
  sya(setvarp, &inter, &dvar);

  out += inter;
  out /= 2;
  sya("n = 0", &inter, &dvar);  
  for(i = 0; i <= number; i++){
    strcpy(setvar2, inp[2]); 
    setvarp2 = mystrcat(setvar2, "+n*((");
    mystrcat(setvarp2, inp[3]);
    mystrcat(setvarp2, "-");
    mystrcat(setvarp2, inp[2]);
    mystrcat(setvarp2, ")/");
    mystrcat(setvarp2, inp[4]);
    mystrcat(setvarp2, ")");
    set_var(setvar, inp[1], setvar2);
    sya(setvarp, &inter, &dvar);
    sya(inp[0], &inter, &dvar);
    out += inter;

    sya("n=n+1", &inter, &dvar);
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


