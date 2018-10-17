#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"


matrix *init_mat(uint16_t *size, uint8_t dim, err_ret *error) {
	matrix *out = malloc(sizeof(*out));
	if(!out) {
		*error = -6;
		return NULL;
	}

	//dim of the matrix
	out->dim = dim;

	//size of each dims
	//the last element of size must end with zero
	out->size = malloc(sizeof(*out->size) * (dim + 1));
	if(!out->size) {
		free(out);
		*error = -6;
		return NULL;
	}

	out->size = memcpy(out->size, size, sizeof(*out->size) * (dim + 1));
	out->size[dim] = 0;

	out->var = 0;

	//get the total length of the array to malloc
	out->len = get_len(size, dim);

	//if there is a 0 element in size
	if(!out->len) {
		*error = -10;
	} else {
		out->elements = calloc(out->len, sizeof(*out->elements));
		if(!out->elements) {
			free(out->size);
			free(out);
			out = NULL;
			*error = -6;
		}
	}

	return out;
}


//define a scalar as just a single dim matrix
//also define that a vector is always 2 dims, with one of
//the two dims being 1
matrix *init_scalar(ele e) {
	matrix *out = malloc(sizeof(*out));
	if(!out)
		return NULL;

	out->dim = 1;
	out->len = 1;

	out->size = malloc(sizeof(*out->size) * 2);
	if(!out->size) {
		free(out);
		return NULL;
	}

	out->size[0] = 1;
	out->size[1] = 0;

	out->elements = malloc(sizeof(*out->elements));
	if(!out->elements) {
		free(out->size);
		free(out);
		return NULL;
	}

	*out->elements = e;

	out->var = 0;

	return out;
}


matrix *cpy_mat(matrix *src, err_ret *error) {
	if(!src)
		return NULL;

	matrix *dest = malloc(sizeof(*dest));
	__MALLOC_CHECK(dest, *error);  

	dest->dim = src->dim;
	dest->len = src->len;
	dest->var = 0;

	dest->elements = malloc(sizeof(*dest->elements) * dest->len);
	__MALLOC_CHECK(dest->elements, *error);  

	dest->elements = memcpy(dest->elements, src->elements,
				sizeof(*dest->elements) * dest->len);

	dest->size = malloc(sizeof(*dest->size) * (dest->dim + 1));
	__MALLOC_CHECK(dest->size, *error);  

	dest->size = memcpy(dest->size, src->size,
			    sizeof(*dest->size) * (dest->dim + 1));
  
	return dest;
}


//a being concatenated to b along dim and sent to out
//the size of out is determined and error checking for sizes of a and b
//is done in concatMatrix, this function only populates the matrix
matrix *assignConcat(matrix *out, matrix *a, matrix *b, uint8_t dim) {
	uint64_t aIncrement = 1;
	uint64_t bIncrement = 1;

	for(uint16_t i = 0; i <= dim; ++i){
		aIncrement *= a->size[i];
		bIncrement *= b->size[i];
	}

	for(uint64_t aIterator = 0, bIterator = 0, sizeOffset = 0, k = 0; k < out->len; ++sizeOffset) {
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


//dim is a number to specifiy along which direction to concatenate
//along, it starts from 0
matrix *cat_mat(matrix *a, matrix *b, uint8_t dim, err_ret *error) {
	int aScalar = is_scalar(a);
	int bScalar = is_scalar(b);

	matrix *out = NULL;

	switch(aScalar + bScalar){

	case 0: //a and b are not scalars
		//a and b are the same dim
		if( !(abs(a->dim - b->dim)) ) {
			uint16_t *sizeA = malloc(sizeof(*sizeA) * (a->dim + 1));
			__MALLOC_CHECK(sizeA, *error);

			uint16_t *sizeB = malloc(sizeof(*sizeB) * (b->dim + 1));
			__MALLOC_CHECK(sizeB, *error);

			sizeA[a->dim] = 0;
			sizeB[b->dim] = 0;

			//creates a new size matrix, that skips the dim that
			//is being concatenated against
			for(int i = 0, j = 0; i < ( a->dim - 1); ++i, ++j){
				if(i == dim){
					++i;
				}

				sizeA[j] = a->size[i];
				sizeB[j] = b->size[i];
			}

			if(cmp_size(sizeA, sizeB, a->dim - 1, b->dim - 1)){
				uint16_t *newSize = malloc(sizeof(*newSize) * (a->dim + 1));
				__MALLOC_CHECK(newSize, *error);

				newSize[a->dim] = 0;

				for(int i = 0; i < a->dim; ++i){
					newSize[i] = a->size[i];
					if(i == dim){
						newSize[i] += b->size[i];
					}
				}

				out = init_mat(newSize, a->dim, error);
				free(newSize);

				assignConcat(out, a, b, dim);

			} else{
				*error = -15;
				out = NULL;
			}

			free(sizeA);
			free(sizeB);
		}

		break;
	case 1: //only one of a or b are scalars
		{
			//temporary variables for less writing in if blocks
			matrix *tempVector = NULL;
			ele tempScalar = 0;
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
			if(!is_vec(tempVector)) {
				*error = -15;
				break;
			}

			//new size vector
			uint16_t newSize[3];
			memcpy(newSize, tempVector->size, sizeof(*newSize) * 3);

			//increment size because of the concatenation
			++newSize[dim];

			//init new matrix
			out = init_mat(newSize, tempVector->dim, error);


			//put values into new matrix
			//first vector values
			for(uint64_t i = 0; i < tempVector->len; ++i)
				out->elements[i + aScalar] = tempVector->elements[i];

			//then scalar value
			//assume that bScalar is either 0 or 1, this then puts
			//the scalar value at either the beginning or the end
			out->elements[tempVector->len * bScalar] = tempScalar;
		}
		break;

	case 2: //both a and b are scalars
		{
			//create and set new size vector

			uint16_t newSize[3] = {1, 1, 0};
			if(dim == 0){
				++newSize[0]; //set newSize to [2, 1]
			} else if(dim == 1){
				++newSize[1]; //set newSize to [1, 2]
			} else{
				*error = 13;
				return NULL;
			}

			out = init_mat(newSize, 2, error);

			out->elements[0] = a->elements[0];
			out->elements[1] = b->elements[0];
		}
		break;

	default: *error = -14; break; //return error if something else
	}

	return out;
}


//free the matrix and all of the data
//associated with it
void free_mat(matrix *m) {
	if(m && !m->var) {
		free(m->size);
		free(m->elements);
		free(m);
	}
}


//prints a matrix that has two dims
//printMatrix works by printing out 2d slices of
//multidimal arrays, offset is going to be
//a multiple of the number of slices of the 2d matrix
//
//for a 3d matrix of size [2 2 2], with elements 1 to 8
//the first print will prpint:
//1 3
//2 4
//the second print will have an offset of 4 and print:
//5 7
//6 8
void print_two_d(const matrix *m, int offset) {

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
void print_mat(const matrix *m) {
	int offset = 0;
	if(m->dim > 2) {
		uint64_t twoDimSize = m->size[0] * m->size[1];
		for(int i = 2; i < m->dim; ++i) {
			for(int j = 0; j < m->size[i]; ++j) {
				print_two_d(m, offset);
				offset += twoDimSize;
			}
		}

	} else if(m->dim == 2){ //2d mat
		print_two_d(m, 0);

	} else{ //scalar
		printf("\n%lf\n\n", m->elements[0]);

	}
	return;
}


uint64_t get_len(uint16_t *size, uint8_t dim) {
	uint64_t out = 1;

	for(uint8_t i = 0; i < dim; ++i)
		out = out * size[i];

	return out;
}


//functions identically to matlab's sub2ind
//converts matrix indexing to linear index given the size of the matrix
//this uses zero indexing
uint64_t sub2ind(uint16_t *location, uint16_t *size, uint8_t dim) {
	uint64_t ind = location[0];
	uint64_t sizeProd = size[0];

	for(int i = 1; i < dim; ++i) {
		ind += (location[i] * sizeProd);
		sizeProd *= size[i];
	}

	return ind;
}


//check if a matrix is a scalar (true)
//else false
err_ret is_scalar(matrix *m) {
	return (m->dim == 1);
}

err_ret is_vec(matrix *m) {
	return (m->dim == 2) && ( (m->size[0] == 1) || (m->size[1] == 1) );
}

//compare two size vectors, return 1 if same
//0 otherwise
err_ret cmp_size(uint16_t *a, uint16_t *b, uint8_t dimA, uint8_t dimB) {
	if(dimA != dimB)
		return 0;

	for(int i = 0; i < dimA; ++i) {
		if(*(a+i) != *(b+i))
			return 0;
	}

	return 1;
}


//check that the inner dims of the matrix match
err_ret chk_inner(matrix *a, matrix *b){
	return (a->size[a->dim - 1] == b->size[0]);
}
