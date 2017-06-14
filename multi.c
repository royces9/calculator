#include <string.h>
#include <stdlib.h>
#include <stdio.h>

double deri(char* inp, char var, double range[], double delta){
}

double inte(char* inp, char var, double range[], double delta){
}

char** sep(char* inp, int* start){

  char sepa[10][1024];
  char *tok;
  int cLEP = 0, cREP = 0, length = 0;
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
  for(int i = 0; tok != NULL; i++){
    strcpy(sepa[i], tok);
    tok = strtok(NULL, ",");
  }

  return sepa;
}


double multifunc(int type, char inp[], int* i){
  int swi = *i;
  char** a;
  switch(swi){
  case 16:
    a = sep(inp, i);
    return deri();
  case 17:
    sep(inp, i);
    return inte();
  default: break;
  }
}


