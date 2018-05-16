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
matrix *initScalar(element e);
matrix *copyMatrix(matrix *src);
matrix *concatMatrix(matrix *a, matrix *b, int dimension, int *error);

void freeMatrix(matrix *m);
void printTwoDMatrix(const matrix m, int offset);
void printMatrix(const matrix m);

int getLength(int *size, int dimension);
int sub2ind(int *location, int *size, int dimension);

int isScalar(matrix *m);

int compareSize(int *a, int *b, int dimA, int dimB);
int checkInnerDim(matrix *a, matrix *b);

#endif //MATRIX
