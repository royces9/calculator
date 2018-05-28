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


//determinies minimum value in the matrix
matrix *min(matrix *m, error_return *error) {
  element out = m->elements[0];

  for(int i = 1; i < m->length; ++i) {
    out = fmin(out, m->elements[i]);
  }

  return initScalar(out, error);
}


//determines maximum value in the matrix
matrix *max(matrix *m, error_return *error) {
  element out = m->elements[0];

  for(int i = 1; i < m->length; ++i) {
    out = fmax(out, m->elements[i]);
  }

  return initScalar(out, error);
}


//calculates average value of the matrix along the last dimensions columns
//ex:
//1 2
//3 4
//should return
//2 3

matrix *avg(matrix *m, error_return *error) {
  matrix *out = NULL;

  int newDimension = m->dimension - 1;
  int *newSize = NULL;

  if(m->dimension == 2){
    newDimension = 2;
    newSize = malloc(sizeof(*newSize) * (newDimension + 1));
    __MALLOC_CHECK(newSize, *error);
    
    newSize[0] = 1;
    newSize[1] = m->size[0];
    newSize[2] = 0;
    
  } else if(m->dimension == 1){

    return copyMatrix(m, error);

  } else{
    newSize = malloc(sizeof(*newSize) * (newDimension + 1));
    __MALLOC_CHECK(newSize, *error);

    newSize = memcpy(newSize, m->size, sizeof(*newSize) * (newDimension + 1));
    newSize[newDimension] = 0;
  }

  out = initMatrix(newSize, newDimension, error);
  free(newSize);

  for(int i = 0; i < out->length; ++i){
    element tempAvg = 0;
    for(int j = 0; j < m->size[m->dimension - 1]; ++j){
      tempAvg += m->elements[j*out->length + i];
    }

    out->elements[i] = tempAvg / m->size[m->dimension - 1];
  }

  return out;
}


//calculates the derivative of a function at a given point with a given step size
matrix *deri(char **input, vari *var, error_return *error) {
  char *str2d;
  element out, inter, h, point;
  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  
  int varIndex = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = point
  input[3] = step size
f   */

  //check the number of inputs is correct
  if(numberOfArgs(input) != 4) {
    *error = -2;
    return 0;
  }


  //set both the point and step size
  *error = sya(input[2], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  point = varTemp->ans->elements[0];

  *error = sya(input[3], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  h = varTemp->ans->elements[0];

  
  //set up a dummy variable specified by user  
  char *dummyVariable = removeSpaces(input[1]);
  varIndex = varcheck(varTemp, dummyVariable);

  if(varIndex == -1){ //if there are no other variables 
    varIndex = 0;
    varTemp->occ = 1;
  } else if(varIndex == -2){ //if there are other variables
    varIndex = ++varTemp->count;
  }

  //set the variable into the local variable struct
  varTemp->name[varIndex] = malloc(sizeof(*varTemp->name[varIndex]) * (strlen(dummyVariable) + 1));
  strcpy(varTemp->name[varIndex], dummyVariable);

  //sets the dummy variable equal to x+h
  varTemp->value[varIndex] = initScalar(point + h, error);
  
  //f(x+h)
  *error = sya(input[0], varTemp);
  if(*error) return 0;
  out = varTemp->ans->elements[0];

  //sets the dummy variable equal to x-h
  varTemp->value[varIndex]->elements[0] = point - h;

  //f(x-h)
  *error = sya(input[0], varTemp);
  if(*error) return 0;
  inter = varTemp->ans->elements[0];

  //f(x+h) - f(x-h)
  out -= inter;
  
  freeVari(varTemp);
  return initScalar(out / (2 * h), error);
}


//calculates integral of a given function with a given range and partition count
matrix *inte(char **input, vari *var, error_return *error) {

  //check number of arguments
  if(numberOfArgs(input) != 5) {
    *error = -2;
    return 0;
  }

  char *str2d;

  element step = 0, sum = 0;
  element out,inter, a, b, number;
  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  int varIndex = 0, iter = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = left bound
  input[3] = right bound
  input[4] = partition count
   */

  //get number of steps, and step size
  *error = sya(input[2], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  a = varTemp->ans->elements[0];

  *error = sya(input[3], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  b = varTemp->ans->elements[0];

  *error = sya(input[4], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  number = varTemp->ans->elements[0];

  //calculate step size
  step = (b - a) / number;


  //set dummy variable
  char *dummyVariable = removeSpaces(input[1]);
  varIndex = varcheck(varTemp, dummyVariable); //checks if variable exists or not, return value used as index
    
  if(varIndex == -1) { //if there are no other variables
    varIndex = 0;
    varTemp->occ = 1;
  } else if(varIndex == -2) { //if there are variables
    varIndex = ++varTemp->count;
  }

  varTemp->name[varIndex] = malloc(sizeof(*varTemp->name[varIndex]) * (strlen(dummyVariable) + 1));
  strcpy(varTemp->name[varIndex],dummyVariable); //copy the dummy variable into struct

  //init scalar for the temp variable
  varTemp->value[varIndex] = initScalar(0, error);


  //calculate integral using composite Simpson's

  number = floor(number/2); //halve the steps

  for(int i = 1; i <= number; ++i) {
    //f(x_2i-2)
    varTemp->value[varIndex]->elements[0] = a + (((2 * i) - 2) * step);
    *error = sya(input[0], varTemp);
    if(*error) return 0;
    sum += varTemp->ans->elements[0];


    //4*f(x_2i-1)
    varTemp->value[varIndex]->elements[0] = a + (((2 * i) - 1) * step);
    *error = sya(input[0], varTemp);
    if(*error) return 0;
    sum += (4 * varTemp->ans->elements[0]);


    //f(x_2i)
    varTemp->value[varIndex]->elements[0] = a + ((2 * i) * step);
    *error = sya(input[0], varTemp);
    if(*error) return 0;
    sum += varTemp->ans->elements[0];
  }

  freeVari(varTemp);

  //return integral
  return initScalar(sum * (step / 3), error);
}


//numerically solve an expression f(x)=0 for x
matrix *solve(char **input, vari *var, error_return *error) {
  //check number of arguments
  if(numberOfArgs(input) != 4) {
    *error = -2;
    return 0;
  }

  char *str2d;
  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  
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
  char *dummyVariable = removeSpaces(input[1]);
  varc = varcheck(varTemp, dummyVariable);

  if(varc == -1) { //if there are no other variables
    varc = 0;
    varTemp->occ = 1;
  } else if(varc == -2) { //if there are other variables
    varc = ++varTemp->count;
  }

  varTemp->name[varc] = malloc(sizeof(*varTemp->name[varc]) * (strlen(dummyVariable) + 1));
  strcpy(varTemp->name[varc], dummyVariable);

  //set initial guess and the tolerance
  *error = sya(input[2], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  varTemp->value[varc] = copyMatrix(varTemp->ans, error);


  *error = sya(input[3], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }
  h = varTemp->ans->elements[0];

  //ensure test is always greater than h
  //on start
  test = h + 1;

  //solve f(x)=0 for x using Newton's method
  while(fabs(test) > h) { //if the difference between iterations is less than the tolerance, break out of loop
    *error = sya(input[0], varTemp);
    if(*error) return NULL;
    out = varTemp->ans->elements[0];

    varTemp->value[varc]->elements[0] -= delta;
    *error = sya(input[0], varTemp);
    if(*error) return NULL;
    inter = varTemp->ans->elements[0];

    test = (delta * out) / (out - inter);
    varTemp->value[varc]->elements[0] -= test;
  }


  matrix *output = copyMatrix(varTemp->value[varc], error);
  freeVari(varTemp);
  return output;
}


matrix *zeros(char **input, vari *var, error_return *error){
  int dimension = numberOfArgs(input);
  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  int *newSize = NULL;

  if(dimension == 1){

    *error = sya(input[0], varTemp);
    if(*error) return NULL;

    if(varTemp->ans->dimension != 1){
      *error = -10;
      freeVari(varTemp);
      return NULL;
    }

    dimension = 2;

    newSize = malloc(sizeof(*newSize) * (dimension + 1));
    if(newSize == NULL){
      *error = -8;
      return NULL;
    }

    newSize[0] = varTemp->ans->elements[0];
    newSize[1] = varTemp->ans->elements[0];

  } else{
    newSize = malloc(sizeof(*newSize) * (dimension + 1));

    for(int i = 0; i < dimension; ++i){
      *error = sya(input[i], varTemp);

      if(varTemp->ans->dimension != 1){
	*error = 10;
	freeVari(varTemp);
	return NULL;
      }

      newSize[i] = varTemp->ans->elements[0];
    }
  }

  freeVari(varTemp);
  newSize[dimension] = 0;

  matrix *out = initMatrix(newSize, dimension, error);

  free(newSize);
  return out;
}


matrix *ones(char **input, vari *var, error_return *error){
  matrix *out = zeros(input, var, error);
  if(*error){
    return out;
  }
  for(int i = 0; i < out->length; ++i){
    out->elements[i] = 1;
  }
  return out;
}


matrix *randMatrix(char **input, vari *var, error_return *error){
  matrix *out = zeros(input, var, error);
  if(*error) return out;

  for(int i = 0; i < out->length; ++i){
    out->elements[i] = (element)rand() / RAND_MAX;
  }

  return out;
}


matrix *linspace(char **input, vari *var, error_return *error){
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

  vari *varTemp = copyVari(var, error);

  element a;
  element b;
  element length;

  *error = sya(input[0], varTemp);
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return out;
  }
  a = varTemp->ans->elements[0];

  *error = sya(input[1], varTemp);
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return out;
  }
  b = varTemp->ans->elements[0];


  *error = sya(input[2], varTemp);
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return out;
  }
  length = varTemp->ans->elements[0];

  int newSize[3] = {length, 1, 0};

  out = initMatrix(newSize, 2, error);

  element step = (b - a) / (length - 1);

  for(int i = 0; i < out->length; ++i){
    out->elements[i] = step * i + a;
  }

  freeVari(varTemp);
  
  return out;
}


matrix *extractValue(char *buffer, char **input, int varIndex, vari *var, error_return *error){
  int varLen = strlen(buffer);
  matrix *out = NULL;

  //for variables that exist
  if(varIndex >= 0){
    int dimension = numberOfArgs(input);
    vari *varTemp = copyVari(var, error);
    matrix *inputMat;

    if(dimension == 1){ //if the number of inputs is 1
      *error = sya(input[0], varTemp);
      if(*error) return NULL;

      out = copyMatrix(varTemp->ans, error);
      if(*error) return NULL;

      //out is a matrix that holds indices
      for(int i = 0; i < out->length; ++i){

	--out->elements[i];

	//check that the input is within bound
	if((int)out->elements[i] >= varTemp->value[varIndex]->length){
	  *error = -11;
	  freeVari(varTemp);
	  freeMatrix(out);

	  return NULL;
	}
      }

    } else if(dimension == varTemp->value[varIndex]->dimension){ //if the number of inputs is equal to dimension
      int *location = malloc(sizeof(*location) * (dimension + 1));

      for(int i = 0; i < dimension; ++i){
	*error = sya(input[i], varTemp);

	//check that the input is one dimensional
	if(varTemp->ans->dimension == 1){
	  //location is 1 indexed, while sub2ind is 0 indexed
	  //so subtract 1 to 0 index
	  location[i] = varTemp->ans->elements[0] - 1;

	  //check that each sublocation is also within bounds
	  if(location[i] >= varTemp->value[varIndex]->size[i]){
	    *error = -11;
	    free(location);
	    freeVari(varTemp);
	    return out;
	  }
	} else{
	  *error = -10;
	  free(location);
	  freeVari(varTemp);
	  return NULL;
	}
      }

      int index = sub2ind(location, varTemp->value[varIndex]->size, varTemp->value[varIndex]->dimension);
      free(location);

      //check index is within bound
      if(index < varTemp->value[varIndex]->length){
	out = initScalar(varTemp->value[varIndex]->elements[index], error);
      } else{
	*error = -11;
      }
      
    } else{
      *error  = -11;
    }

    freeVari(varTemp);
  } else {
    *error = -5;
  }

  return out;
}

//remove spaces from char input
char *removeSpaces(char *input) {
  int length = strlen(input);

  int i = 0;
  int j = 0;

  for(; *(input + i) == ' '; ++i);

  for(; *((input + length) - j) == ' '; -- j);

  *((input + length) - j) = '\0';

  return (input + i);
}

//print a line to stdout, formatting is similar to matlab
error_return printLine(char **input, vari *var) {
  error_return error = 0;
  int argNo = numberOfArgs(input), front = 0, back = 0;
  vari *varTemp = copyVari(var, &error);

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
      error = sya(input[i], varTemp); //calculate expression and print, print variables this way
      if(error) return error;
      printMatrix(*varTemp->ans);

    }
  }

  freeVari(varTemp);

  //-1 is the error code for no output from sya
  return -1;
}

//separate a single string into multiple strings by a given delimiter
char **separateString(char *input, char limits[2], char delimiter, int *start, error_return *error) {
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
