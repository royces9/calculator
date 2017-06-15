#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "strucon.h"
#include "sya.h"
#include "funcs.h"

double deri(char inp[10][1024], vari* var){
  double *ans;
  
  sya(inp[1], ans, var);
}

double inte(char inp[10][1024], vari* var){
  double *ans;

  
}

void sep(char* inp, int* start, char sepa[10][1024]){

  char *tok;
  int cLEP = 0, cREP = 0, length = 0, i = 0;
  inp += *start;

  for(length = 0; inp[length]; length++){
    if(inp[length] == '('){
      cLEP++;
    }
    else if(inp[length] == ')'){
      cREP++;
    }

    if(cLEP == cREP){
      break;
    }

  }

  inp[length+1] = 0;

  tok = strtok(inp, ",");
  for(i = 0; tok != NULL; i++){
    strcpy(sepa[i], tok);
    tok = strtok(NULL, ",");
    i++;
  }
  strcpy(sepa[i], "");
}


 double multifunc(int type, char inp[], int* i, vari* var){
  int swi = *i;
  char sepa[10][1024];

  switch(swi){
  case 16:
    sep(inp, i, sepa);
    return deri(sepa, var); 
  case 17:
    sep(inp, i, sepa);
    return inte(sepa, var);
  default: break;
  }
}


