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


//calculates the derivative of a function at a given point with a given step size
matrix *deri(char **input, vari *var, error_return *error) {
  element out, inter, h, point;
  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  
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

  *error = setVariable(varTemp, dummyVariable, initScalar(point + h, error), &varIndex);
  
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

  element step = 0, sum = 0;
  element out, inter, a, b, number;
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

  *error = setVariable(varTemp, dummyVariable, initScalar(0, error), &varIndex);    

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

  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  
  element out, inter, h;
  double test = 0, delta = 0.000001;
  int varIndex = 0;

  /*
  input[0] = function
  input[1] = variable
  input[2] = initial guess
  input[3] = tolerance
   */

  //set dummy variable

  //set initial guess and the tolerance
  *error = sya(input[2], varTemp);
  if(*error) return 0;
  if(varTemp->ans->dimension != 1){
    *error = -10;
    return NULL;
  }


  char *dummyVariable = removeSpaces(input[1]);
  varIndex = varcheck(varTemp, dummyVariable);

  *error = setVariable(varTemp, dummyVariable, copyMatrix(varTemp->ans, error), &varIndex);    


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

  //counter to keep track of the amount of iterations
  //if it overflows, then break from the loop
  uint16_t counter = 1;

  //solve f(x)=0 for x using Newton's method
  while(fabs(test) > h) { //if the difference between iterations is less than the tolerance, break out of loop
    *error = sya(input[0], varTemp);
    if(*error) return NULL;
    out = varTemp->ans->elements[0];

    varTemp->value[varIndex]->elements[0] -= delta;
    *error = sya(input[0], varTemp);
    if(*error) return NULL;
    inter = varTemp->ans->elements[0];

    test = (delta * out) / (out - inter);
    varTemp->value[varIndex]->elements[0] -= test;

    //if counter overflows and goes back to 0
    //this is true, max value is 65535 (2 bytes)
    if(!(++counter)){
      *error = -12;
      return NULL;
    }
  }


  matrix *output = copyMatrix(varTemp->value[varIndex], error);
  freeVari(varTemp);
  return output;
}


matrix *zeros(char **input, vari *var, error_return *error){
  int dimension = numberOfArgs(input);
  vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  int *newSize = NULL;


  //only one input, make a square matrix of that size
  if(dimension == 1){

    *error = sya(input[0], varTemp);
    if(*error) return NULL;

    //check that the one input is a scalar
    if(varTemp->ans->dimension != 1){
      *error = -10;
      freeVari(varTemp);
      return NULL;
    }

    //change dimension to make square matrix
    dimension = 2;

    newSize = malloc(sizeof(*newSize) * (dimension + 1));
    if(newSize == NULL){
      *error = -8;
      return NULL;
    }

    newSize[0] = varTemp->ans->elements[0];
    newSize[1] = varTemp->ans->elements[0];

  } else{ //
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


//make a matrix of all ones
matrix *ones(char **input, vari *var, error_return *error){
  //call zeros and just replace all the input
  matrix *out = zeros(input, var, error);

  if(*error) return out;

  for(int i = 0; i < out->length; ++i){
    out->elements[i] = 1;
  }

  return out;
}


//make a matrix of random numbers from 0 to 1
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

  element a = 0;
  element b = 0;
  element length = 0;

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

    if(input[0][0] == 0){
      *error = -4;
      return NULL;
    }
    
    vari *varTemp = copyVari(var, error);
    matrix *inputMat;


    if(dimension == 1){ //if the number of inputs is 1
      *error = sya(input[0], varTemp);
      if(*error){
	freeVari(varTemp);
	return NULL;
      }

      out = copyMatrix(varTemp->ans, error);
      if(*error){
	freeVari(varTemp);
	return NULL;
      }

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
	if(*error){
	  free(location);
	  freeVari(varTemp);
	  return NULL;
	}

	//check that the input is one dimensional
	if(varTemp->ans->dimension == 1){
	  //location is 1 indexed, while sub2ind is 0 indexed
	  //so subtract 1 to 0 index
	  location[i] = varTemp->ans->elements[0] - 1;

	  //check that each sublocation is also within bounds
	  if((location[i] >= varTemp->value[varIndex]->size[i]) || (location[i] < 0)){
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
      location[dimension] = 0;

      int index = sub2ind(location, varTemp->value[varIndex]->size, varTemp->value[varIndex]->dimension);
      free(location);

      //check index is within bound
      if(index < varTemp->value[varIndex]->length){
	out = initScalar(index, error);
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


//buffer -  holds the character buffer from sya
//tok - is a pointer which determines whether '-' is negative or subtraction
//input -  is the entire input string
//iterator -  is the counter for the main loop in sya
error_return checkVariable(const char *buffer, int *tok, char *input, int *iterator, vari *var, numberStack *num, operatorStack *ch){

  error_return error = 0;

  int varLen = strlen(buffer);

  char *nameBuffer = malloc(sizeof(*nameBuffer) * (varLen + 1));
  __MALLOC_CHECK(nameBuffer, error);

  //copy buffer so we don't change the original string
  strcpy(nameBuffer, buffer);

  int k = 0;

  matrix *out = NULL;
  char **separatedString = NULL;


  if(nameBuffer[varLen - 1] == '('){

    nameBuffer[varLen - 1] = '\0';

    k = varcheck(var, nameBuffer);
    if(k < 0){
      free(nameBuffer);
      return -5;
    }

    separatedString = separateString(input, "()", ",", iterator, &error);

    out = extractValue(nameBuffer, separatedString, k, var, &error);

    if(!(error < 0)){
      pushn(var->value[k], num);
      pushn(out, num);

      pushch(initOperatorStruct("r", 2, 0, eReference), ch);
    }

    freeDoubleArray(separatedString);

  } else{
    k = varcheck(var, nameBuffer);

    if(k >= 0) {
      var->value[k]->variable = 1;
      pushn(var->value[k], num);
      *tok = 1;

    } else{
      if(k == -1){
	k = 0;

      } else if(k == -2){
	k = var->count + 1;

      } else{
	error = -5;

      }

      //if assignment goes wrong, the variable name gets malloc'd
      //but doesn't get assigned to, this ensures that if another
      //assignment happens, the previous failed assignment is free'd
      if(var->name[k] != NULL){
	free(var->name[k]);
	free(var->value[k]);
      }

      var->name[k] = malloc(sizeof(*var->name[k]) * (varLen + 1));
      __MALLOC_CHECK(var->name[k], error);

      strcpy(var->name[k], nameBuffer);

      var->value[k] = malloc(sizeof(*var->value[k]));
      __MALLOC_CHECK(var->value[k], error);

      var->value[k]->variable = 1;
      var->value[k]->size = NULL;
      var->value[k]->elements = NULL;

      pushn(var->value[k], num);
    }
  }

  free(nameBuffer);

  return error;
}


//remove spaces from char input
char *removeSpaces(char *input) {
  int length = strlen(input);

  int i = 0;
  int j = 0;

  for(; *(input + i) == ' '; ++i);

  for(j = length - 1; input[j] == ' '; --j){
    input[j] = 0;
  }


  return (input + i);
}


//print a line to stdout, formatting is similar to matlab
error_return printLine(char **input, vari *var) {
  error_return error = 0;

  int argNo = numberOfArgs(input);
  int front = 0;
  int back = 0;

  vari *varTemp = copyVari(var, &error);

  //loop over every argument
  for(int i = 0; i < argNo; ++i) {
    int len = strlen(input[i]);

    //check if the string is quote limited
    int string = 0;

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


    //if there are quotes at the beginning/end of string, this is true
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
    } else { //no quotes, just a variable or expression
      error = sya(input[i], varTemp); //calculate expression and print, print variables this way
      if(error) return error;

      printMatrix(varTemp->ans);

    }
  }

  freeVari(varTemp);

  //-1 is the error code for no output from sya
  return -1;
}


//separate a string
//input: string to separate, string comes from sya input
//limiter: things like parenthesis or brackets, outer most limiter is assumed to be ()
//delimiter: the different types of chars that may be delimiting the string
//iterator: offset of input to the first parenthesis

char **separateString(char *input, char const * const limiter, char const * const delimiter, int *iterator, error_return *error) {

  //increment input to the first parenthesis
  input += (*iterator + 1);

  uint16_t length = 0;
  //find where the parenthesis are closed
  //can assume that they are closed correctly
  for(int16_t a = 0; input[length]; ++length){
    a += (input[length] == '(');
    a -= (input[length] == ')');

    //if a is 0, break
    if(!a){
      break;
    }
  }

  //increment iterator length amount, to the
  //char after the end paren
  *iterator += (length + 1);

  //get a copy of the input to mangle
  char *input2 = calloc(length + 1, sizeof(*input2));
  __MALLOC_CHECK(input2, *error);
  strncpy(input2, input + 1, length - 1);

  //the number of types of delimiters
  uint8_t delimiterType = strlen(delimiter);

  //the number of delimiters in input
  uint16_t delimiterCount = 0;

  //number of types of limiters
  //assume that the number of limiters is going to be even
  //there will always be a left and right end
  uint8_t limiterType = strlen(limiter) / 2;

  //count the total number of delimiters
  for(uint16_t i = 0; input2[i]; ++i){
    for(uint8_t j = 0; j < delimiterType; ++j){
      if(input[i] == delimiter[j]){
	++delimiterCount;
	break;
      }
    }
  }


  //assume that each delimiter will have its own string
  //also account for an end NULL pointer
  char **separatedString = malloc(sizeof(*separatedString) * (delimiterCount + 2));
  __MALLOC_CHECK(separatedString, *error);

  //count of the number of elements
  uint16_t subString = 0;

  if(!delimiterCount){
    separatedString[0] = malloc(sizeof(**separatedString) * (length + 1));
    __MALLOC_CHECK(separatedString[0], *error);    
    strcpy(*separatedString, input2);

  } else{

    //counter for each of the limiters
    int16_t *limiterCount = calloc(limiterType, sizeof(*limiterCount));
    __MALLOC_CHECK(limiterCount, error);

    //last index where a delimiter was found
    uint16_t currentLength = 0;


    //loop counter
    uint16_t k = 0;
    for(; input2[k]; ++k){
      //byte to check that all the limiters are balanced
      uint8_t limiterOr = 0;

      //check that each limiter is balanced
      //limiterCount is 0 if the pair is balanced
      for(uint8_t l = 0; l < limiterType; ++l){
	limiterCount[l] += (input2[k] == limiter[l]);
	limiterCount[l] -= (input2[k] == limiter[l+1]);

	limiterOr |= limiterCount[l];
      }

      //if the current char is a delimiter
      //and the limiters are balanced, means
      //that the delimiter delimits the arguments
      //of the main function, and not arguments of
      //any sub functions within
      if(strchr(delimiter, input2[k]) && !limiterOr){
	//malloc the number of characters in between each delimiter
	separatedString[subString] = malloc(sizeof(**separatedString) * ((k - currentLength)) + 1);
	__MALLOC_CHECK(separatedString[subString], *error);

	//copy from the previous delimiter to the next one
	strncpy(separatedString[subString], input2 + currentLength, k - currentLength);

	//null terminate string
	separatedString[subString][k - currentLength] = '\0';

	//set the current delimiter to the one just found
	currentLength = k;

	//increment the number of subStrings
	++subString;
      }
    }

    free(limiterCount);

    separatedString[subString] = malloc(sizeof(**separatedString) * ((k - currentLength) + 1));
    __MALLOC_CHECK(separatedString[subString], *error);
    strncpy(separatedString[subString], input2 + currentLength + 1, k - currentLength);
    separatedString[subString][k - currentLength] = '\0';
  }

  separatedString[++subString] = NULL;

  free(input2);

  return separatedString;
}

/*
char **separateString(char *input, const char limits[2], const char delimiter, int *iterator, error_return *error) {
  char *tok;

  //counter for left and right bounds
  int leftLimit = 0;
  int rightLimit = 0;

  //length of string
  int length = 0;

  //counter for number of delimiters
  int delimiterCount = 0;

  //delimiter string
  char strDelimiter[2] = {delimiter, 0};
  
  input += (*iterator + 1);
  
  if(limits != NULL){
    for(; input[length]; ++length) {
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
  strncpy(input2, input, sizeof(*input2) * (length + 3));
  input2[length] = 0;

  //allocate double array output and populate it with strings
  char **separatedString = malloc((delimiterCount + 2) * sizeof(*separatedString));
  __MALLOC_CHECK(separatedString, *error);

  *iterator += (length+1);
  tok = strtok(input2, strDelimiter);

  //fill the first index with a string
  separatedString[0] = malloc((strlen(tok) + 1) * sizeof(**separatedString));
  __MALLOC_CHECK(separatedString[0], *error);
  strcpy(separatedString[0], ++tok);

  tok = strtok(NULL, strDelimiter);

  int i = 1;
  //every loop populates separateString with another string
  for(; tok != NULL; ++i) {
    separatedString[i] = malloc((strlen(tok) + 1) * sizeof(**separatedString));
    __MALLOC_CHECK(separatedString[i], *error);
    strcpy(separatedString[i], tok);
    tok = strtok(NULL, strDelimiter);
  }

  separatedString[i] = NULL;

  free(input2);
  return separatedString;
}
*/
