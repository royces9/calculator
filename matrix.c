#include "matrix.h"
#include "stack.h"

matrix *initMatrix(int *size, int dimension, int *error){
  matrix *out;

  //dimension of the matrix
  out.dimension = dimension;

  //size of each dimensions
  out.size = size;

  //get the total length of the array to malloc
  out.length = getLength(size);

  //if there is a 0 element in size
  if(!out.length){
    //temp value for the error
    *error = -10;
    return;
  }
  out.elements = calloc(sizeof(out.elements) * out.length);
  __MALLOC_CHECK(out.elements, *error);
  return out;
}

void freeMatrix(matrix *m){
  free(m->elements);
  free(m->size);
}

void printMatrix(matrix *m){
}

int getLength(int *size){
  int temp = 1;
  for(int i = 0; size; ++i){
    temp = temp * size[i];
  }
  return temp;
}

//functions identically to matlab's sub2ind
int sub2ind(int *location, int *size, int dimension){
  int length = getLength(int *size);

}
