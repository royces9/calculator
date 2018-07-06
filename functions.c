#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "operator.h"
#include "functions.h"

//factorial function
element factorial(element a) {
  a = floor(a);
  if(a == 0) {
    return 1;
  }
  return a == 1 ? 1 : a * factorial(a - 1);
}


matrix *eye(matrix *a, error_return *error){
  if(a->dimension != 1){
    *error = -12;
    return NULL;
  }

  uint16_t newSize[3] = {a->elements[0], a->elements[0], 0};
  matrix *out = initMatrix(newSize, 2, error);

  int index = 0;
  for(int i = 0; i < a->elements[0]; ++i){
    index = i * (a->elements[0] + 1);
    out->elements[index] = 1;
  }

  return out;
}


matrix *getSize(matrix *a, error_return *error){
  uint16_t newSize[3];

  //output is a row vector
  newSize[0] = 1;
  newSize[1] = a->dimension;
  newSize[2] = 0;
  
  matrix *out = initMatrix(newSize, 2, error);
  for(int i = 0; a->size[i]; ++i){
    out->elements[i] = a->size[i];
  }

  return out;
}


matrix *magnitude(matrix *a, error_return *error){
  matrix *out = NULL;

  if(a->dimension != 2){
    *error = -10;

  } else{
    if((a->size[0] == 1) || (a->size[1] == 1)){
      element magnitudeA = 0;

      for(int i = 0; i < a->length; ++i){
	magnitudeA += (a->elements[i] * a->elements[i]);
      }

      magnitudeA = sqrt(magnitudeA);

      out = initScalar(magnitudeA, error);
    } else{
      *error = -10;
    }
  }

  return out;
}


//get the total number of elements of a
matrix *numel(matrix *a, error_return *error){
  matrix *out = initScalar(a->length, error);
  return out;
}

/*function for matrix referencing
ex:
a = linspace(0,1,3)
    0, 0.5, 1
b = a(2) + 1
    1.5

a - the matrix
b - the index
*/
matrix *reference(matrix *a, matrix *b, error_return *error){
  matrix *out = copyMatrix(b, error);

  for(int i = 0; i < b->length; ++i){
    out->elements[i] = a->elements[(uint64_t) (b->elements[i])];
  }

  return out;
}


/*
a - pointer to variable (left side of '=')
b - pointer to new value (right side of '=')
*/
matrix *assign(matrix *a, matrix *b, vari *var, error_return *error){

  uint8_t incrementFlag = 1;
  if(a->variable){
    if(var->assignIndex == NULL){
      //init new matrix
      //copyMatrix not done because the
      //pointer 'a' is malloc'd in findFunction

      if(a->size != NULL){
	free(a->size);
	free(a->elements);
	incrementFlag = 0;
      }
      
      a->length = b->length;
      a->dimension = b->dimension;


      a->elements = malloc(sizeof(*a->elements) * a->length);
      __MALLOC_CHECK(a->elements, *error);

      memcpy(a->elements, b->elements, sizeof(*a->elements) * a->length);


      a->size = malloc(sizeof(*a->size) * (a->dimension + 1));
      __MALLOC_CHECK(a->size, *error);

      memcpy(a->size, b->size, sizeof(*a->size) * (a->dimension + 1));

    } else{

      for(int i = 0; i < var->assignIndex->length; ++i){
	a->elements[(int) var->assignIndex->elements[i]] = b->elements[i];
      }

      incrementFlag = 0;
      freeMatrix(var->assignIndex);
      var->assignIndex = NULL;

    }
  } else{
    *error = -5;
    return NULL;
  }


  if(incrementFlag){
    ++var->count;
  }

  return copyMatrix(a, error);
}


matrix *divideMatrix(matrix *a, matrix *b, error_return *error){
  return NULL;
}


matrix *multiplyMatrix(matrix *a, matrix *b, error_return *error){
  matrix *out = NULL;

  //matrix multiplication only defined for 2d arrays
  if((a->dimension != 2) || (b->dimension != 2)){
    *error = -10;
    return NULL;
  }

  //check that the inner dimensions match
  if(a->size[1] != b->size[0]){
    *error = -10;
    return NULL;
  }

  uint16_t newSize[3] = {a->size[0], b->size[1], 0};
  out = initMatrix(newSize, 2, error);

  matrix *transposeA = transposeMatrix(a, error);

  //counter for elements put into out
  int l = 0;

  //transpose a and then multiply every column with every other column in each matrix
  for(int i = 0; i < b->size[1]; ++i){
    for(int j = 0; j < transposeA->size[1]; ++j){
      element tempSum = 0;
      for(int k = 0; k < transposeA->size[0]; ++k){
	tempSum += transposeA->elements[k + j * transposeA->size[0]] * b->elements[k + i * b->size[0]];
      }
      out->elements[l] = tempSum;
      ++l;
    }
  }
    
  freeMatrix(transposeA);
  return out;
}


matrix *exponentMatrix(matrix *a, matrix *b, error_return *error){
  int aScalar = isScalar(a);
  int bScalar = isScalar(b);

  matrix *tempMat = NULL;
  matrix *out = NULL;

  switch(aScalar + bScalar){
  case 0: //neither a nor b are scalars
    *error = -10;
    return out;

  case 1: //one of a or b is a scalar
    if(aScalar){ //a is the scalar
      out = copyMatrix(b, error);

      for(int i = 0; i < out->length; ++i){
	out->elements[i] = pow(a->elements[0],b->elements[i]);
      }

    } else{ //b is the scalar
      //check that b is a whole number, no imaginary numbers (yet?)

      out = initScalar(a->size[0], error);
      tempMat = eye(out, error);
      freeMatrix(out);
      //really small number

      if((b->elements[0] - floor(b->elements[0])) < 0.00000000001){
	long int power = b->elements[0];
	for(int i = 0; i < power; ++i){
	  out = multiplyMatrix(tempMat, a, error);
	  freeMatrix(tempMat);

	  if(*error){
	    freeMatrix(out);
	    return NULL;
	  }

	  tempMat = copyMatrix(out, error);
	  freeMatrix(out);
	}
      }

      return tempMat;
    }

    return out;


  case 2: //a and b are both scalar
    out = initScalar(pow(a->elements[0], b->elements[0]), error);
    return out;


  default: *error = -10; return out;
  }
}


matrix *transposeMatrix(matrix *a, error_return *error){
  //transpose only defined for 2d matrix
  if(a->dimension != 2){
    *error = -10;
    return NULL;
  }

  //new transposed size is same as a->size
  //but the dimensions are swapped
  uint16_t newSize[3] = {a->size[1], a->size[0], 0};
  matrix *out = initMatrix(newSize, 2, error);

  //new index
  int newInd = 0;

  //temp variable for the sublocation of the new index
  int subLoc = 0;
  
  for(int i = 0; i < out->length; ++i){
    //subLoc is an int and gets rounded down
    subLoc = i/a->size[0];
    newInd = subLoc + a->size[1] * (i - subLoc * a->size[0]);

    out->elements[newInd] = a->elements[i];
  }

  return out;
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


//sums along the last dimension of the matrix
matrix *sum(matrix *m, error_return *error){
  matrix *out = NULL;

  int newDimension = m->dimension - 1;
  uint16_t *newSize = NULL;

  if(m->dimension == 2){

    //for vector
    if((m->size[0] == 1) || (m->size[1] == 1)){
      newSize = malloc(sizeof(*newSize) * (newDimension + 1));
      __MALLOC_CHECK(newSize, *error);

    } else{
      newDimension = 2;
      newSize = malloc(sizeof(*newSize) * (newDimension + 1));
      __MALLOC_CHECK(newSize, *error);
      newSize[1] = m->size[0];

    }
    newSize[0] = 1;

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
    element tempSum = 0;

    for(int j = 0; j < m->size[m->dimension - 1]; ++j){
      tempSum += m->elements[i*out->length + j];
    }

    out->elements[i] = tempSum;
  }

  return out;
}

//calculates average value of the matrix along the last dimensions columns
//ex:
//1 2
//3 4
//should return
//2 3
matrix *avg(matrix *m, error_return *error) {
  matrix *out = sum(m, error);

  for(int i = 0; i < out->length; ++i){
    out->elements[i] /= m->size[m->dimension - 1];
  }

  return out;
}


//returns value from one argument functions
element oneArg(element a, int o, error_return *error) {
switch(o) {
 case eSin: return sin(a);
 case eCos: return cos(a);
 case eTan: return tan(a);
 case eLn: return log(a);
 case eLog: return log10(a);
 case eSqrt: return sqrt(a);
 case eAsin: return asin(a);
 case eAcos: return acos(a);
 case eAtan: return atan(a);
 case eFloor: return floor(a);
 case eCeil: return ceil(a);
 case eRound: return round(a);
 case eFactorial: return factorial(a);
 default: *error = 1; return a;
}
}


//returns value from two argument function
element twoArg(element a, element b, int o, error_return *error) {
  switch(o) {
  case eAdd: return a + b;
  case eSubtract: return a - b;
  case eMultiply: return a * b;
  case eDivide: return a / b;
  case eExponent: return pow(a, b);
  case eLess: return a < b;
  case eGreater: return a > b;
  case eLessEqual: return a <= b;
  case eGreaterEqual: return a >= b;
  case eNotEqual: return a != b;
  case eEqual: return a == b;
  case eAnd: return a && b;
  case eOr: return a || b;
  default: *error = 1; return a;
  }
}
