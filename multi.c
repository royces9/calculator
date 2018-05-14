#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stack.h"
#include "multi.h"
#include "operator.h"
#include "sya.h"

//counts number of input arguments
int numberOfArgs(char **input) {
  int i = 0;
  for(; input[i]; ++i); //empty for
  return i;
}


//determines minimum value from the inputs given
matrix *min(matrix *m, int *error) {
  element out = m->elements[0];
  for(int i = 1; i < m->length; ++i) {
    out = fmin(out, m->elements[i]);
  }
  return initScalar(out);
}


//determines maximum value from the inputs given
matrix *max(matrix *m, int *error) {
  element out = m->elements[0];
  for(int i = 1; i < m->length; ++i) {
    out = fmax(out, m->elements[i]);
  }
  return initScalar(out);
}


//calculates average value from the inputs given
matrix *avg(matrix *m, int *error) {
  element sum = 0;
  for(int i = 0; i < m->length; ++i) {
    sum += m->elements[i];
  }
  return initScalar(sum/m->length);
}


//calculates the derivative of a function at a given point with a given step size
matrix *deri(char **input, vari *var, int *error) {
  char *str2d;
  element out, inter, h, point;
  vari varTemp = copyVari(var); //copy global struct to a local variable struct
  
  int varIndex = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = point
  input[3] = step size
   */

  //check the number of inputs is correct
  if(numberOfArgs(input) != 4) {
    *error = -2;
    return 0;
  }


  //set both the point and step size
  *error = sya(input[2], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  point = varTemp.ans.elements[0];

  *error = sya(input[3], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  h = varTemp.ans.elements[0];

  
  //set up a dummy variable specified by user  
  varIndex = varcheck(&varTemp, input[1]);

  if(varIndex == -1){ //if there are no other variables 
    varIndex = 0;
    varTemp.occ = 1;
  } else if(varIndex == -2){ //if there are other variables
    varIndex = ++varTemp.count;
  }

  //set the variable into the local variable struct
  strcpy(varTemp.name[varIndex], input[1]);

  //sets the dummy variable equal to x+h
  varTemp.value[varIndex] = initScalar(point + h);
  
  //f(x+h)
  *error = sya(input[0], &varTemp);
  if(*error) return 0;
  out = varTemp.ans.elements[0];

  //sets the dummy variable equal to x-h
  varTemp.value[varIndex]->elements[0] = point - h;

  //f(x-h)
  *error = sya(input[0], &varTemp);
  if(*error) return 0;
  inter = varTemp.ans.elements[0];

  //f(x+h) - f(x-h)
  out -= inter;
  
  freeVari(&varTemp);
  return initScalar(out / (2 * h));
}


//calculates integral of a given function with a given range and partition count
matrix *inte(char **input, vari *var, int *error) {

  //check number of arguments
  if(numberOfArgs(input) != 5) {
    *error = -2;
    return 0;
  }


  char *str2d;

  element step = 0, sum = 0;
  element out,inter, a, b, number;
  vari varTemp = copyVari(var); //copy global struct to a local variable struct
  int varIndex = 0, iter = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = left bound
  input[3] = right bound
  input[4] = partition count
   */

  //get number of steps, and step size
  *error = sya(input[2], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  a = varTemp.ans.elements[0];

  *error = sya(input[3], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  b = varTemp.ans.elements[0];

  *error = sya(input[4], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  number = varTemp.ans.elements[0];

  //calculate step size
  step = (b - a) / number;


  //set dummy variable
  varIndex = varcheck(&varTemp, input[1]); //checks if variable exists or not, return value used as index
    
  if(varIndex == -1) { //if there are no other variables
    varIndex = 0;
    varTemp.occ = 1;
  } else if(varIndex == -2) { //if there are variables
    varIndex = ++varTemp.count;
  }
  strcpy(varTemp.name[varIndex],input[1]); //copy the dummy variable into struct

  //init scalar for the temp variable
  varTemp.value[varIndex] = initScalar(0);


  //calculate integral using composite Simpson's

  number = floor(number/2); //halve the steps

  for(int i = 1; i <= number; ++i) {
    //f(x_2i-2)
    varTemp.value[varIndex]->elements[0] = a + (((2 * i) - 2) * step);
    *error = sya(input[0], &varTemp);
    if(*error) return 0;
    sum += varTemp.ans.elements[0];


    //4*f(x_2i-1)
    varTemp.value[varIndex]->elements[0] = a + (((2 * i) - 1) * step);
    *error = sya(input[0], &varTemp);
    if(*error) return 0;
    sum += (4 * varTemp.ans.elements[0]);


    //f(x_2i)
    varTemp.value[varIndex]->elements[0] = a + ((2 * i) * step);
    *error = sya(input[0], &varTemp);
    if(*error) return 0;
    sum += varTemp.ans.elements[0];
  }

  freeVari(&varTemp);

  //return integral
  return initScalar(sum * (step / 3));
}


//numerically solve an expression f(x)=0 for x
matrix *solve(char **input, vari *var, int *error) {
  //check number of arguments
  if(numberOfArgs(input) != 4) {
    *error = -2;
    return 0;
  }

  char *str2d;
  vari varTemp = copyVari(var); //copy global struct to a local variable struct
  
  element out, inter, h;
  double test = 0, delta = 0.000001;
  int varc = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = initial guess
  input[3] = tolerance
   */

  //set dummy variable
  varc = varcheck(&varTemp, input[1]);

  if(varc == -1) { //if there are no other variables
    varc = 0;
    varTemp.occ = 1;
  } else if(varc == -2) { //if there are other variables
    varc = ++varTemp.count;
  }
  strcpy(varTemp.name[varc],input[1]);

  //set initial guess and the tolerance
  *error = sya(input[2], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  varTemp.value[varc] = copyMatrix(&varTemp.ans);


  *error = sya(input[3], &varTemp);
  if(*error) return 0;
  if(varTemp.ans.dimension != 1){
    *error = -10;
    return NULL;
  }
  h = varTemp.ans.elements[0];

  //ensure test is always greater than h
  //on start
  test = h + 1;

  //solve f(x)=0 for x using Newton's method
  while(fabs(test) > h) { //if the difference between iterations is less than the tolerance, break out of loop
    *error = sya(input[0], &varTemp);
    if(*error) return 0;
    out = varTemp.ans.elements[0];

    varTemp.value[varc]->elements[0] -= delta;
    *error = sya(input[0], &varTemp);
    if(*error) return 0;
    inter = varTemp.ans.elements[0];

    test = (delta * out) / (out - inter);
    varTemp.value[varc]->elements[0] -= test;
  }

  freeVari(&varTemp);
  return varTemp.value[varc];
}


matrix *zeros(char **input, vari *var, int *error){
  int dimension = numberOfArgs(input);
  int *size = malloc(sizeof(*size) * (dimension + 1));

  vari varTemp = copyVari(var);; //copy global struct to a local variable struct

  for(int i = 0; i < dimension; ++i){
    *error = sya(input[i], &varTemp);
    matrix *inputMat = copyMatrix(&varTemp.ans);

    if(inputMat->dimension != 1){
      *error = 13;
      freeVari(&varTemp);
      return NULL;
    }
    size[i] = inputMat->elements[0];
    freeMatrix(inputMat);
  }

  size[dimension] = 0;
  freeVari(&varTemp);
  matrix *out = initMatrix(size, dimension, error);

  free(size);
  return out;
}


matrix *ones(char **input, vari *var, int *error){
  matrix *out = zeros(input, var, error);
  if(*error){
    return out;
  }
  for(int i = 0; i < out->length; ++i){
    out->elements[i] = 1;
  }
  return out;
}


matrix *linspace(char **input, vari *var, int *error){
  int argNo = numberOfArgs(input);

  /*
input[0] = left limit
input[1] = right limit
input[2] = number of elements
   */

  matrix *out = NULL;

  if(argNo != 3){
    *error = -2;
    return out;
  }

  vari temp = copyVari(var);

  element a;
  element b;
  element length;

  *error = sya(input[0], &temp);
  if(temp.ans.dimension != 1){
    *error = -10;
    return out;
  }
  a = temp.ans.elements[0];

  *error = sya(input[1], &temp);
  if(temp.ans.dimension != 1){
    *error = -10;
    return out;
  }
  b = temp.ans.elements[0];


  *error = sya(input[2], &temp);
  if(temp.ans.dimension != 1){
    *error = -10;
    return out;
  }
  length = temp.ans.elements[0];

  int newSize[3] = {length, 1, 0};

  out = initMatrix(newSize, 2, error);

  element step = (b - a) / (length - 1);

  for(int i = 0; i < out->length; ++i){
    out->elements[i] = step * i + a;
  }

  freeVari(&temp);
  
  return out;
}

matrix *extractValue(char *buffer, char **input, vari *var, int *error){
  int varLen = strlen(buffer);
  matrix *out;

  //change the '(' to a 0
  buffer[varLen-1] = '\0';

  int varIndex = varcheck(var, buffer);
  if(varIndex >= 0){
    int dimension = numberOfArgs(input);
    vari varTemp = copyVari(var);
    matrix *inputMat;

    if(dimension == 1){
      *error = sya(input[0], &varTemp);
      out = copyMatrix(&varTemp.ans);
      //inputMat = copyMatrix(&varTemp.ans);
      //out = copyMatrix(inputMat);

      for(int i = 0; i < out->length; ++i){

	if((int)out->elements[i] <= varTemp.value[varIndex]->length){
	  out->elements[i] = varTemp.value[varIndex]->elements[(int)out->elements[i] - 1];

	} else{
	  *error = -14;
	  freeVari(&varTemp);
	  freeMatrix(out);

	  return NULL;
	}
      }


      freeVari(&varTemp);
      return out;

    } else if(dimension == varTemp.ans.dimension){
      int *location = malloc(sizeof(*location) * (dimension + 1));

      for(int i = 0; i < dimension; ++i){
	*error = sya(input[i], &varTemp);
	if(varTemp.ans.dimension == 1){
	  location[i] = varTemp.ans.elements[0];
	} else{
	  free(location);
	  freeVari(&varTemp);
	  freeMatrix(out);
	  return NULL;
	}
      }

      int index = sub2ind(location, varTemp.value[varIndex]->size, varTemp.value[varIndex]->dimension);
      free(location);

      if(index < varTemp.value[varIndex]->length){
	out = initScalar(varTemp.value[varIndex]->elements[index]);
      } else{
	freeVari(&varTemp);
	freeMatrix(out);
	return NULL;
      }

      return out;
      
    } else{
      *error  = -14;
      freeVari(&varTemp);
      return NULL;
    }

    freeVari(&varTemp);
  }

  return out;
}

//remove spaces from char input
void removeSpaces(char *input, int *front, int *back) {
  if(input[0] == ' ') {
    int i = 1;
    for(i = 1; input[i] != ' '; ++i);
    *front = i;
  }

  int len = strlen(input);
  if(input[len-1] == ' ') {
    int i = 1;
    for(i = 1; input[len - i] != ' '; ++i);
    *back = i;
  }
}

//print a line to stdout, formatting is similar to matlab
int printLine(char **input, vari *var, int *error) {
  int argNo = numberOfArgs(input), front = 0, back = 0;
  vari varTemp = copyVari(var);

  for(int i = 0; i < argNo; ++i) {
    int len = strlen(input[i]), string = 0;

    //check if there is a quote in beginning of string, or spaces then a quote
    if(input[i][0] == '"') {
      string = 1;
    } else{
      for(front = 0; input[i][front] == ' '; ++front) {
	if(input[i][front+1] == '"') {
	  string = 1;
	}
      }
    }

    //check if there is a quote at the end of the string, or spaces then a quote (going backwards
    if(input[i][len-(back+1)] == '"') {
      ++string;
    } else {
      for(back = 0; input[i][len-(back+1)] == ' '; ++back) {
	if(input[i][len-(back+1)] == '"') {
	  ++string;
	}
      }
    }

    //if there are quotes the the beginning/end of string, this is true
    if(string) {
      input[i][len-(back+1)] = '\0'; //null terminate it to write over the quote
      if(string == 2) {
	if((input[i][len-(back+3)] == '\\') && (input[i][len-(back+2)] == 'n')) { //check if there's a new line
	  input[i][len-(back+3)] = '\0'; //write over the \ in new line
	  printf("%s\n", input[i]+front+1); //print with new line
	} else {
	  printf("%s", input[i]+front+1); //print without new line
	}
      } else {
	return -9; //there's no second quote to match, error
      }
    }
    else { //no quotes, just a variable or expression
      *error = sya(input[i], &varTemp); //calculate expression and print, print variables this way
      if(*error) return 0;
      printMatrix(varTemp.ans);

    }
  }

  freeVari(&varTemp);

  return 0;
}

//separate a single string into multiple strings by a given delimiter
char **separateString(char *input, char limits[2], char delimiter, int *start, int *error) {
  char *tok;
  int leftLimit = 0, rightLimit = 0, length = 0, delimiterCount = 0, i = 0;  

  char strDelimiter[2] = {delimiter, 0};
  
  input += (*start+1);
  
  if(limits != NULL){
    for(length = 0; input[length]; ++length) {
      //increment count if char is left or right end parenthesis
      leftLimit += (input[length] == limits[0]);
      rightLimit += (input[length] == limits[1]);
      //increment count if char is the delimiter
      delimiterCount += (input[length] == delimiter);

      if(leftLimit == rightLimit) {
	break;
      }
    }
  } else{
    length = strlen(input);
  }

  //temp variable that strtok will take in, since strtok mangles original pointer

  char *input2 = malloc((length + 3)* sizeof(*input2));
  __MALLOC_CHECK(input2, *error);

  //only copy up to length # of characters
  strncpy(input2,input, sizeof(*input2) * (length + 3));
  input2[length] = 0;

  //allocate double array output and populate it the strings
  char **separatedString = malloc((delimiterCount + 2) * sizeof(*separatedString));
  __MALLOC_CHECK(separatedString, *error);

  *start += (length+1);
  tok = strtok(input2, strDelimiter);

  //fill the first index with a string
  separatedString[0] = malloc((strlen(tok) + 1) * sizeof(**separatedString));
  __MALLOC_CHECK(separatedString[0], *error);
  strcpy(separatedString[0], ++tok);

  tok = strtok(NULL, strDelimiter);

  //every loop populates separateString with another string
  for(i = 1; tok != NULL; ++i) {
    separatedString[i] = malloc((strlen(tok) + 1) * sizeof(**separatedString));
    __MALLOC_CHECK(separatedString[i], *error);
    strcpy(separatedString[i], tok);
    tok = strtok(NULL, strDelimiter);
  }

  separatedString[i] = NULL;
  //separatedString[i] = malloc(sizeof(**separatedString)); //allocate an end string that is just a char
  //__MALLOC_CHECK(separatedString[i], *error);

  //separatedString[i][0]= '\0'; //end string

  free(input2);
  return separatedString;
}
