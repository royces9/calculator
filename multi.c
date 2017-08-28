#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stack.h"
#include "funcs.h"
#include "multi.h"
#include "sya.h"

#define __SYA_ERROR(error) \
  if(error){ return 0;}

int numberOfArgs(char **input){
  int i = 0;
  for(i = 0; strcmp(input[i], "\0"); i++);
  return i;
}

double min(char **input, vari *var, int *error){
  char *str2d, *str2d2;
  double out, inter;
  int argNo = numberOfArgs(input);

  *error = sya(input[0], &out, var);
  __SYA_ERROR(*error);

  *error = sya(input[1], &inter, var);
  __SYA_ERROR(*error);

  out = fmin(out, inter);
  for(int i = 2; i < argNo; i++){
    sya(input[i], &inter, var);
    __SYA_ERROR(*error);
    out = fmin(out, inter);
  }
  return out;
}

double max(char **input, vari *var, int *error){
  double out, inter;
  int argNo = numberOfArgs(input);

  *error = sya(input[0], &out, var);
  __SYA_ERROR(*error);

  *error = sya(input[1], &inter, var);
  __SYA_ERROR(*error);

  out = fmax(out, inter);
  for(int i = 2; i < argNo; i++){
    *error = sya(input[i], &inter, var);
    __SYA_ERROR(*error);
    out = fmax(out, inter);
  }
  return out;
}

double avg(char **input, vari *var, int *error){
  double sum = 0;
  double partSum = 0;
  int argNo = numberOfArgs(input);
  for(int i = 0; i < argNo; i++){
    sya(input[i], &partSum, var);
    __SYA_ERROR(*error);
    sum += partSum;
  }
  return sum/argNo;
}

double deri(char **input, vari *var, int *error){
  char *str2d;
  double out = 0, inter = 0, point = 0, h = 0;
  vari varTemp = *var;
  int varIndex = 0;
  //  int startTime = clock()/CLOCKS_PER_SEC;

  /*
  input[0] = function
  input[1] = variable
  input[2] = point
  input[3] = tolerance 
   */

  //check the number of inputs is correct
  if(numberOfArgs(input) != 4){
    *error = -2;
    return 0;
  }

  *error = sya(input[2], &point, &varTemp);
    __SYA_ERROR(*error);

  *error = sya(input[3], &h, &varTemp);
    __SYA_ERROR(*error);
  
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
  *error = sya(input[0], &out, &varTemp);
  __SYA_ERROR(*error);

  if(*error){
    return 0;
  }

  //sets the dummy variable equal to x-h
  varTemp.value[varIndex] = point - h;

  //does f(x-h)
  *error = sya(input[0], &inter, &varTemp);
  __SYA_ERROR(*error);

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

  /*
  input[0] = function
  input[1] = variable
  input[2] = left bound
  input[3] = right bound
  input[4] = partition count
   */

  //check number of arguments
  if(numberOfArgs(input) != 5){
    *error = -2;
    return 0;
  }

  //get number of steps, and step size
  *error = sya(input[2], &a, &varTemp);
  __SYA_ERROR(*error);

  *error = sya(input[3], &b, &varTemp);
  __SYA_ERROR(*error);

  *error = sya(input[4], &number, &varTemp);
  __SYA_ERROR(*error);

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
    *error = sya(input[0], &out, &varTemp);
    __SYA_ERROR(*error);

    sum += out;

    varTemp.value[varIndex] = a + (((2 * i) - 1) * step);
    *error = sya(input[0], &inter, &varTemp);
    __SYA_ERROR(*error);

    sum += (4 * inter);

    varTemp.value[varIndex] = a + ((2 * i) * step);
    *error = sya(input[0], &out, &varTemp);
    __SYA_ERROR(*error);

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

  /*
  input[0] = function
  input[1] = variable
  input[2] = initial guess
  input[3] = tolerance
   */

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

  *error = sya(input[2], &varTemp.value[varc], &varTemp);
  __SYA_ERROR(*error);

  *error = sya(input[3], &h, &varTemp);
  __SYA_ERROR(*error);

  test = h + 1; //ensure test is always greater than h

  while(fabs(test) > h){
    *error = sya(input[0],&out, &varTemp);
    __SYA_ERROR(*error);

    varTemp.value[varc] -= delta;
    *error = sya(input[0],&inter, &varTemp);
    __SYA_ERROR(*error);

    test = (delta*out)/(out-inter);
    varTemp.value[varc] -= test;
  }
  return varTemp.value[varc];
}


char **separateString(char input[], char delimiter, int *start, int *error){

  char *tok;
  int leftParenthesisCount = 0, rightParenthesisCount = 0, length = 0, delimiterCount = 0, i = 0;  

  char strDelimiter[2];
  strDelimiter[0] = delimiter;
  strDelimiter[1] = '\0';
  
  input += (*start+1);

  for(length = 0; input[length]; length++){
    if(input[length] == '('){
      leftParenthesisCount++;
    }
    else if(input[length] == ')'){
      rightParenthesisCount++;
    }
    if(input[length] == delimiter){
      delimiterCount++;
    }
    if(leftParenthesisCount == rightParenthesisCount){
      break;
    }
  }

  char *input2 = malloc((length + 2)* sizeof(*input2));
  __MALLOC_CHECK(input2, *error);
  strcpy(input2,input);

  //allocate double array output
  char **separatedString = malloc((delimiterCount + 2) * sizeof(*separatedString));
  __MALLOC_CHECK(separatedString, *error);

  for(int j = 0; j < (delimiterCount + 2); j++){
    separatedString[j] = malloc(length * sizeof(**separatedString));
    __MALLOC_CHECK(*separatedString, *error);
  }

  *start += (length+1);
  input2[length+1] = 0;

  tok = strtok(input2, strDelimiter);
  ++tok;

  for(i = 0; tok != NULL; i++){
    strcpy(separatedString[i], tok);
    tok = strtok(NULL, strDelimiter);
  }

  separatedString[i-1][strlen(separatedString[i-1])-1] = '\0';

  strcpy(separatedString[i], "");
  free(input2);

  return separatedString;
}
