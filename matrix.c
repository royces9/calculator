#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"


matrix *initMatrix(int *size, int dimension, int *error){
  matrix *out = malloc(sizeof(*out));

  //dimension of the matrix
  out->dimension = dimension;

  //size of each dimensions
  //the last element of size must end with zero
  out->size = malloc(sizeof(*out->size) * dimension);
  out->size = memcpy(out->size, size, sizeof(*out->size) * dimension);

  //get the total length of the array to malloc
  out->length = getLength(size, dimension);
  
  //if there is a 0 element in size
  if(!out->length){
    //temp value for the error
    *error = -10;
    return NULL;
  }

  out->elements = calloc(out->length, sizeof(*out->elements));
  //__MALLOC_CHECK(out->elements, *error);
  return out;
}

void freeMatrix(matrix *m){
  free(m->size);
  free(m->elements);
  free(m);
}


//offset is the number of elements to offset the dimension m
void printTwoDMatrix(matrix *m, int offset){
  
  int newSize[2] = {m->size[0], m->size[1]};
  for(int i = 0; i < m->size[0]; ++i){
    for(int j = 0; j < m->size[1]; ++j){
      int location[2] = {i, j};
      int ind = sub2ind(location, newSize, m->dimension);
      printf("%lf ", m->elements[offset+sub2ind(location, newSize, 2)]);
    }
    printf("\n");
  }
  printf("\n");
  return;
}

void printMatrix(matrix *m){
  int offset = 0;
  int twoDimSize = m->size[0] * m->size[1];
  if(m->dimension >2){
    for(int i = 1; i < m->dimension; ++i){
	printTwoDMatrix(m, offset);
	offset += twoDimSize;
    }
  }
  else{
    printTwoDMatrix(m, 0);
  }
  return;
}

int getLength(int *size, int dimension){
  int out = 1;
  for(int i = 0; i < dimension; ++i){
    out = out * size[i];
  }
  return out;
}


//functions identically to matlab's sub2ind
//converts matrix indexing to linear index given the size of the matrix
int sub2ind(int *location, int *size, int dimension){
  int ind = location[0] + size[0];
  int sizeProd = size[0];
  for(int i = 1; i < dimension; ++i){
    ind = ind + sizeProd * (location[i] - 1);
    sizeProd *= size[i];
  }
  return ind;
}
