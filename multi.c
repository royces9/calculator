#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stack.h"
#include "funcs.h"
#include "multi.h"
#include "sya.h"

int numberOfArgs(char **input){
  int i = 0;
  for(i = 0; strcmp(input[i], "\0"); i++);
  return i;
}

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

double min(char **input, vari *var, int *error){
  char *str2d, *str2d2;
  double out, inter;
  int argNo = numberOfArgs(input);
  
  out = vartypeset(var, input[0]);
  inter = vartypeset(var, input[1]);

  out = fmin(out, inter);
  for(int i = 2; i < argNo; i++){
    inter = vartypeset(var, input[i]);
    out = fmin(out, inter);
  }
  return out;
}

double max(char **input, vari *var, int *error){
  double out, inter;
  int argNo = numberOfArgs(input);
  
  out = vartypeset(var, input[0]);
  inter = vartypeset(var, input[1]);

  out = fmax(out, inter);
  for(int i = 2; i < argNo; i++){
    inter = vartypeset(var, input[i]);
    out = fmax(out, inter);
  }
  return out;
}

double avg(char **input, vari *var, int *error){
  double sum = 0;
  int argNo = numberOfArgs(input);
  for(int i = 0; i < argNo; i++){
    sum += vartypeset(var, input[i]);
  }
  return sum/argNo;
}

double deri(char **input, vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, point = 0, h = 0;
  vari varTemp = *var;
  int varIndex = 0;
  //  int startTime = clock()/CLOCKS_PER_SEC;

  //check the number of inputs is correct
  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }

  point = vartypeset(&varTemp, input[2]);
  h = vartypeset(&varTemp, input[3]);
  
  //set up a dummy variable specified by user  
  varIndex = varcheck(&varTemp, input[1]);

  if(varIndex == -1){
    varIndex = 0;
    varTemp.occ = 1;
  }
  else if(varIndex == -2){
    varIndex = ++varTemp.count;
  }

  strcpy(varTemp.name[varIndex], input[1]);

  //sets the dummy variable equal to x+h
  varTemp.value[varIndex] = point + h;
  
  //does f(x+h)
  sya(input[0], &out, &varTemp);

  //sets the dummy variable equal to x-h
  varTemp.value[varIndex] = point - h;

  //does f(x-h)
  sya(input[0], &inter, &varTemp);

  //this is f(x+h) - f(x-h)
  out -= inter;

  //  int endTime = clock()/CLOCKS_PER_SEC;
  //  printf("time: %d", endTime-startTime);
  return out/(2*h);
}

double inte(char **input, vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, step = 0, number = 0, a = 0, b = 0, sum = 0, halfnumber = 0;
  vari varTemp = *var;
  int varIndex = 0;
  //  double startTime = clock()/(double)CLOCKS_PER_SEC;
  //check number of arguments
  if(numberOfArgs(input) != 5){
    *error = -2;
    return 0;
  }

  //get number of steps, and step size
  a = vartypeset(&varTemp, input[2]);
  b = vartypeset(&varTemp, input[3]);
  number = vartypeset(&varTemp, input[4]);
  step = (b-a)/number;

  //set dummy variable
  varIndex = varcheck(&varTemp, input[1]); //checks if variable exists or not, return value used as index
  
  if(varIndex == -1){
    varIndex = 0;
    varTemp.occ = 1;
  }
  else if(varIndex == -2){
    varIndex == ++varTemp.count;
  }
  strcpy(varTemp.name[varIndex],input[1]); //copy the dummy variable into struct

  //calculate integral using composite Simpson's
  if(fmod(number,2)){
    number++;
  }
  halfnumber = number/2;

  for(int i = 1; i <= halfnumber; i++){
    varTemp.value[varIndex] = a + (((2 * i) - 2) * step);
    sya(input[0], &out, &varTemp);
    sum += out;

    varTemp.value[varIndex] = a + (((2 * i) - 1) * step);
    sya(input[0], &inter, &varTemp);
    sum += (4 * inter);

    varTemp.value[varIndex] = a + ((2 * i) * step);
    sya(input[0], &out, &varTemp);
    sum += out;
  }
  //  double endTime = clock() / (double)CLOCKS_PER_SEC;
  //  printf("time: %lf", endTime-startTime);

  //return integral
  return sum * (step / 3);
}

double solve(char **input, vari *var, int *error){
  char *str2d;
  vari varTemp = *var;
  double out = 0, inter = 0, h = 0, test = 0, delta = 0.000001;
  int varc = 0;

  //check number of arguments
  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }

  //set dummy variable
  varc = varcheck(&varTemp, input[1]);

  if(varc == -1){
    varc = 0;
    varTemp.occ = 1;
  }
  else if(varc == -2){
    varc = ++varTemp.count;
  }
  strcpy(varTemp.name[varc],input[1]);
  varTemp.value[varc] = vartypeset(&varTemp, input[2]);

  h = vartypeset(&varTemp, input[3]);
  test = h + 1; //ensure test is always greater than h

  while(fabs(test) > h){
    sya(input[0],&out, &varTemp);
    varTemp.value[varc] -= delta;
    sya(input[0],&inter, &varTemp);
    test = (delta*out)/(out-inter);
    varTemp.value[varc] -= test;
  }
  return varTemp.value[varc];
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
