#ifndef MATRIX
#define MATRIx

typedef struct{
  int length; //number of elements 
  int dimension; //number of dimensions
  int *size; //size of the array
  double *elements; //elements of vector
} matrix; 


matrix *initMatrix(int *size, int dimension);
void freeMatrix(matrix *m);
void printMatrix(matrix *m);

int getLength(int *size);
int sub2ind(int *location, int *size);




#endif //MATRIX
