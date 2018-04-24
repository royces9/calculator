#ifndef MATRIX
#define MATRIx

typedef struct matrix{
  int length; //number of elements 
  int dimension; //number of dimensions
  int *size; //size of the array
  double *elements; //elements of vector
} matrix; 


matrix *initMatrix(int *size, int dimension, int *error);
void freeMatrix(matrix *m);
void printTwoDMatrix(matrix *m, int offset);
void printMatrix(matrix *m);

int getLength(int *size, int dimension);
int sub2ind(int *location, int *size, int dimension);




#endif //MATRIX
