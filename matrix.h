#ifndef MATRIX
#define MATRIx


//typedef double to element
typedef double element;

//typedef int8_t to error return type
typedef int8_t error_return;

typedef struct matrix{
  //number of elements;
  int length;

  //number of dimensions
  int dimension;

  //size of the array
  int *size;

  //elements of the matrix
  element *elements;

  //variable flag
  int8_t variable;
} matrix; 


matrix *initMatrix(int *size, int dimension, error_return *error);
matrix *initScalar(element e, error_return *error);
matrix *copyMatrix(matrix *src, error_return *error);
matrix *concatMatrix(matrix *a, matrix *b, int dimension, error_return *error);

void freeMatrix(matrix *m);
void printTwoDMatrix(const matrix m, int offset);
void printMatrix(const matrix m);

int getLength(int *size, int dimension);
int sub2ind(int *location, int *size, int dimension);

int8_t isScalar(matrix *m);

int8_t compareSize(int *a, int *b, int dimA, int dimB);
int8_t checkInnerDim(matrix *a, matrix *b);

#endif //MATRIX
