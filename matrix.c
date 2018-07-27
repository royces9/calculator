#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"


matrix *initMatrix(uint16_t *size, uint8_t dimension, error_return *error) {
	matrix *out = malloc(sizeof(*out));
	__MALLOC_CHECK(out, *error);  

	//dimension of the matrix
	out->dimension = dimension;

	//size of each dimensions
	//the last element of size must end with zero
	out->size = malloc(sizeof(*out->size) * (dimension + 1));
	__MALLOC_CHECK(out->size, *error);  

	out->size = memcpy(out->size, size, sizeof(*out->size) * (dimension + 1));
	out->size[dimension] = 0;

	out->variable = 0;

	//get the total length of the array to malloc
	out->length = getLength(size, dimension);

	//if there is a 0 element in size
	if(!out->length) {
		*error = -10;
	} else {
		out->elements = calloc(out->length, sizeof(*out->elements));
		__MALLOC_CHECK(out->elements, *error);  
	}

	return out;
}


//define a scalar as just a single dimension matrix
//also define that a vector is always 2 dimensions, with one of
//the two dimensions being 1
matrix *initScalar(element e, error_return *error) {
	matrix *out = malloc(sizeof(*out));
	__MALLOC_CHECK(out, *error);  

	out->dimension = 1;
	out->length = 1;

	out->size = malloc(sizeof(*out->size) * 2);
	__MALLOC_CHECK(out->size, *error);  

	out->size[0] = 1;
	out->size[1] = 0;

	out->elements = malloc(sizeof(*out->elements));
	__MALLOC_CHECK(out->elements, *error);  

	*out->elements = e;

	out->variable = 0;

	return out;
}


matrix *copyMatrix(matrix *src, error_return *error) {
	if(!src)
		return NULL;

	matrix *dest = malloc(sizeof(*dest));
	__MALLOC_CHECK(dest, *error);  

	dest->dimension = src->dimension;
	dest->length = src->length;
	dest->variable = 0;

	dest->elements = malloc(sizeof(*dest->elements) * dest->length);
	__MALLOC_CHECK(dest->elements, *error);  

	dest->elements = memcpy(dest->elements, src->elements, sizeof(*dest->elements) * dest->length);

	dest->size = malloc(sizeof(*dest->size) * (dest->dimension + 1));
	__MALLOC_CHECK(dest->size, *error);  

	dest->size = memcpy(dest->size, src->size, sizeof(*dest->size) * (dest->dimension + 1));
  
	return dest;
}


//a being concatenated to b along dimension and sent to out
//the size of out is determined and error checking for sizes of a and b
//is done in concatMatrix, this function only populates the matrix
matrix *assignConcat(matrix *out, matrix *a, matrix *b, uint8_t dimension) {
	int aIncrement = 1;
	int bIncrement = 1;

	for(int i = 0; i <= dimension; ++i){
		aIncrement *= a->size[i];
		bIncrement *= b->size[i];
	}

	for(int aIterator = 0, bIterator = 0, sizeOffset = 0, k = 0; k < out->length; ++sizeOffset) {
		for(aIterator = 0; aIterator < aIncrement; ++aIterator) {
			out->elements[k] = a->elements[aIterator + aIncrement * sizeOffset];
			++k;
		}
      
		for(bIterator = 0; bIterator < bIncrement; ++bIterator) {
			out->elements[k] = b->elements[bIterator + bIncrement * sizeOffset];
			++k;
		}
	}

	return out;
}


//dimension is a number to specifiy along which direction to concatenate
//along, it starts from 0
matrix *concatMatrix(matrix *a, matrix *b, uint8_t dimension, error_return *error) {
	int aScalar = isScalar(a);
	int bScalar = isScalar(b);

	matrix *out = NULL;

	switch(aScalar + bScalar){

	case 0: //a and b are not scalars
		//a and b are the same dimension
		if( !(abs(a->dimension - b->dimension)) ) {
			uint16_t *sizeA = malloc(sizeof(*sizeA) * (a->dimension + 1));
			__MALLOC_CHECK(sizeA, *error);

			uint16_t *sizeB = malloc(sizeof(*sizeB) * (b->dimension + 1));
			__MALLOC_CHECK(sizeB, *error);

			sizeA[a->dimension] = 0;
			sizeB[b->dimension] = 0;

			//creates a new size matrix, that skips the dimension that
			//is being concatenated against
			for(int i = 0, j = 0; i < ( a->dimension - 1); ++i, ++j){
				if(i == dimension){
					++i;
				}

				sizeA[j] = a->size[i];
				sizeB[j] = b->size[i];
			}

			if(compareSize(sizeA, sizeB, a->dimension - 1, b->dimension - 1)){
				uint16_t *newSize = malloc(sizeof(*newSize) * (a->dimension + 1));
				__MALLOC_CHECK(newSize, *error);

				newSize[a->dimension] = 0;

				for(int i = 0; i < a->dimension; ++i){
					newSize[i] = a->size[i];
					if(i == dimension){
						newSize[i] += b->size[i];
					}
				}

				out = initMatrix(newSize, a->dimension, error);
				free(newSize);

				assignConcat(out, a, b, dimension);

			} else{
				*error = -15;
				out = NULL;
			}

			free(sizeA);
			free(sizeB);

			break;
      
			case 1: //only one of a or b are scalars
				{
					//temporary variables for less writing in if blocks
					matrix *tempVector = NULL;
					element tempScalar = 0;
					//assign which matrix is a scalar and which is a matrix
					if(aScalar){
						tempVector = b;
						tempScalar = a->elements[0];

					} else{
						tempVector = a;
						tempScalar = b->elements[0];

					}

					//check that the matrix is a vector, only vectors can be
					//concatenated with scalars
					if((tempVector->dimension > 2) ||
					   (tempVector->size[!dimension] != 1)) {
						*error = -15;
						break;
					}

					//new size vector
					uint16_t newSize[3];
					memcpy(newSize, tempVector->size, sizeof(*newSize) * 3);

					//increment size because of the concatenation
					++newSize[dimension];

					//init new matrix
					out = initMatrix(newSize, tempVector->dimension, error);


					//put values into new matrix
					//first vector values
					for(int i = 0; i < tempVector->length; ++i) {
						out->elements[i + aScalar] = tempVector->elements[i];
					}

					//then scalar value
					//assume that bScalar is either 0 or 1, this then puts
					//the scalar value at either the beginning or the end
					out->elements[tempVector->length * bScalar] = tempScalar;
				}
				break;


				case 2: //both a and b are scalars
					{
						//create and set new size vector

						uint16_t newSize[2] = {1, 1};
						if(dimension == 0){
							++newSize[0]; //set newSize to [2, 1]
						} else if(dimension == 1){
							++newSize[1]; //set newSize to [1, 2]
						} else{
							*error = 13;
							return NULL;
						}

						out = initMatrix(newSize, 2, error);

						out->elements[0] = a->elements[0];
						out->elements[1] = b->elements[0];
					}
					break;

					default: *error = -14; break; //return error if something else
		}
	}

	return out;
}


//free the matrix and all of the data
//associated with it
void freeMatrix(matrix *m) {
	if(!m->variable) {
		free(m->size);
		free(m->elements);
		free(m);
	}
}


//prints a matrix that has two dimensions
//printMatrix works by printing out 2d slices of
//multidimensional arrays, offset is going to be
//a multiple of the number of slices of the 2d matrix
//
//for a 3d matrix of size [2 2 2], with elements 1 to 8
//the first print will prpint:
//1 3
//2 4
//the second print will have an offset of 4 and print:
//5 7
//6 8
void printTwoDMatrix(const matrix *m, int offset) {

	printf("\n");
	for(int columns = 0; columns < m->size[0]; ++columns) {
		for(int rows = 0; rows < m->size[1]; ++rows) {
			//the below is the same as sub2ind for a 2d matrix
			//columns + m.size[0] * rows
			printf("%lf ", m->elements[offset + (columns + m->size[0] * rows)]);
		}
		printf("\n");
	}
	printf("\n");

	return;
}


//print out a matrix of any size
//prints out 2d slices of the matrix
void printMatrix(const matrix *m) {
	int offset = 0;
	if(m->dimension > 2) {
		uint64_t twoDimSize = m->size[0] * m->size[1];
		for(int i = 2; i < m->dimension; ++i) {
			for(int j = 0; j < m->size[i]; ++j) {
				printTwoDMatrix(m, offset);
				offset += twoDimSize;
			}
		}

	} else if(m->dimension == 2){ //2d mat
		printTwoDMatrix(m, 0);

	} else{ //scalar
		printf("\n%lf\n\n", m->elements[0]);

	}
	return;
}


uint64_t getLength(uint16_t *size, uint8_t dimension) {
	uint64_t out = 1;
	for(uint8_t i = 0; i < dimension; ++i) {
		out = out * size[i];
	}

	return out;
}


//functions identically to matlab's sub2ind
//converts matrix indexing to linear index given the size of the matrix
//this uses zero indexing
uint64_t sub2ind(uint16_t *location, uint16_t *size, uint8_t dimension) {
	uint64_t ind = location[0];
	uint64_t sizeProd = size[0];

	for(int i = 1; i < dimension; ++i) {
		ind += (location[i] * sizeProd);
		sizeProd *= size[i];
	}

	return ind;
}


//check if a matrix is a scalar (true)
//else false
error_return isScalar(matrix *m) {
	return (m->dimension == 1);
}

error_return isVector(matrix *m) {
	return (m->dimension == 2) && ( (m->size[0] == 1) || (m->size[1] == 1) );
}

//compare two size vectors, return 1 if same
//0 otherwise
error_return compareSize(uint16_t *a, uint16_t *b, uint8_t dimA, uint8_t dimB) {
	if(dimA != dimB)
		return 0;

	for(int i = 0; i < dimA; ++i) {
		if(*(a+i) != *(b+i))
			return 0;
	}

	return 1;
}


//check that the inner dimensions of the matrix match
error_return checkInnerDim(matrix *a, matrix *b){
	return (a->size[a->dimension-1] == b->size[0]) ? 1 : 0;
}
