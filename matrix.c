#include "matrix.h"
#include "stack.h"

matrix *initMatrix(int *size, int dimension, int *error){
  matrix *out;

  //dimension of the matrix
  out->dimension = dimension;

  //size of each dimensions
  //the last element of size must end with zero
  out->size = size;

  //get the total length of the array to malloc
  out->length = getLength(size);

  //if there is a 0 element in size
  if(!out->length){
    //temp value for the error
    *error = -10;
    return;
  }
  out->elements = calloc(sizeof(out->elements) * out->length);
  __MALLOC_CHECK(out->elements, *error);
  return out;
}

void freeMatrix(matrix *m){
  free(m->elements);
  free(m->size);
}

void printMatrix(matrix *m){
  for(int i = 0; i < m->dimensions; ++i){
    for(int j = 0; j < m->size[i]; ++j){
    }
  }
}

int getLength(int *size){
  int out = 1;
  for(int i = 0; size; ++i){
    out = out * size[i];
  }
  return out;
}


//functions identically to matlab's sub2ind
//converts matrix indexing to linear index given the size of the matrix
int sub2ind(int *location, int *size, int dimension){
  int ind = location[0];
  int sizeProd = size[0];
  for(int i = 1; size; ++i){
    ind = ind + sizeProd * (location[i] - 1);
    sizeProd *= size[i];
  }
  return ind;
}
