#ifndef MATRIX
#define MATRIx

//typedef double to element
typedef double element;

//typedef int8_t to error return type
typedef int8_t error_return;

typedef struct matrix{
  //number of elements;
  uint64_t length;

  //number of dimensions
  uint8_t dimension;

  //size of the array
  uint16_t *size;

  //elements of the matrix
  element *elements;

  //variable flag
  int8_t variable;
} matrix; 


matrix *initMatrix(uint16_t *size, uint8_t dimension, error_return *error);
matrix *initScalar(element e, error_return *error);
matrix *copyMatrix(matrix *src, error_return *error);
matrix *concatMatrix(matrix *a, matrix *b, uint8_t dimension, error_return *error);

void freeMatrix(matrix *m);
void printTwoDMatrix(const matrix *m, int offset);
void printMatrix(const matrix *m);

uint64_t getLength(uint16_t *size, uint8_t dimension);
uint64_t sub2ind(uint16_t *location, uint16_t *size, uint8_t dimension);

error_return isScalar(matrix *m);

error_return compareSize(uint16_t *a, uint16_t *b, uint8_t dimA, uint8_t dimB);
error_return checkInnerDim(matrix *a, matrix *b);

#endif //MATRIX
