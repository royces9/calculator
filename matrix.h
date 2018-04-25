#ifndef MATRIX
#define MATRIx

//typedef double to element
typedef double element;

typedef struct matrix{
  int length; //number of elements 
  int dimension; //number of dimensions
  int *size; //size of the array
  element *elements; //elements of vector
} matrix; 


matrix *initMatrix(int *size, int dimension, int *error);
void freeMatrix(matrix *m);
void printTwoDMatrix(matrix *m, int offset);
void printMatrix(matrix *m);

int getLength(int *size, int dimension);
int sub2ind(int *location, int *size, int dimension);

matrix *matrixOneArg(matrix *a, int o);
matrix *matrixTwoArg(matrix *a, matrix *b, int o);

int matrixOperator(int o);

int compareSize(int *a, int *b, int dim);
int checkInnerDim(matrix *a, matrix *b);

#endif //MATRIX
