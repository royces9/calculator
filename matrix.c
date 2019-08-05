#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

//struct matrix *init_mat(uint16_t *size, uint8_t dim, err_ret *error) {
err_ret init_mat(uint16_t *size, uint8_t dim, struct matrix **out) {
	*out = malloc(sizeof(**out));
	if(!*out) {
		return e_malloc;
	}

	//dim of the matrix
	out[0]->dim = dim;

	//size of each dims
	//the last element of size must end with zero
	out[0]->size = malloc((dim + 1) * sizeof(*(out[0]->size)));
	if(!out[0]->size) {
		free(out);
		return e_malloc;
	}

	memcpy(out[0]->size, size, (dim + 1) * sizeof(*(out[0]->size)));
	out[0]->size[dim] = 0;

	out[0]->var = 0;

	//get the total length of the array to malloc
	out[0]->len = get_len(size, dim);

	//if there is a 0 element in size
	if(!out[0]->len)
		return e_size_mismatch;

	out[0]->elements = calloc(out[0]->len, sizeof(*(out[0]->elements)));
	if(!out[0]->elements) {
		free(out[0]->size);
		free(*out);
		return e_malloc;
	}

	return 0;
}


//define a scalar as just a single dim matrix
//also define that a vector is always 2 dims, with one of
//the two dims being 1
err_ret init_scalar(ele e, struct matrix **out) {
	*out = malloc(sizeof(**out));
	if(!(*out))
		return e_malloc;

	out[0]->dim = 1;
	out[0]->len = 1;

	if( !(out[0]->size = malloc(2 * sizeof(*(out[0]->size)))) )
		return e_malloc;

	out[0]->size[0] = 1;
	out[0]->size[1] = 0;

	if( !(out[0]->elements = malloc(sizeof(*(out[0]->elements)))) )
		return e_malloc;

	out[0]->elements[0] = e;

	out[0]->var = 0;

	return 0;
}


err_ret cpy_mat(struct matrix const *const src, struct matrix **out) {
	if(!src)
		return e_malloc;

	struct matrix *dest = malloc(sizeof(*dest));
	if( !dest )
		return e_malloc;

	dest->dim = src->dim;
	dest->len = src->len;
	dest->var = 0;

	if( !(dest->elements = malloc(dest->len * sizeof(*dest->elements))) )
		return e_malloc;

	dest->elements = memcpy(dest->elements, src->elements,
				dest->len * sizeof(*dest->elements));

	if( !(dest->size = malloc((dest->dim + 1) * sizeof(*dest->size))) )
		return e_malloc;

	dest->size = memcpy(dest->size, src->size,
			    (dest->dim + 1) * sizeof(*dest->size));
  
	*out = dest;

	return 0;
}


//a being concatenated to b along dim and sent to out
//the size of out is determined and error checking for sizes of a and b
//is done in concatMatrix, this function only populates the matrix
struct matrix *assignConcat(struct matrix *out, struct matrix const *const a,
			    struct matrix const *const b, uint8_t dim) {
	uint64_t aIncr = 1;
	uint64_t bIncr = 1;

	for(uint16_t i = 0; i <= dim; ++i){
		aIncr *= a->size[i];
		bIncr *= b->size[i];
	}

	for(uint64_t aIter = 0, bIter = 0, sizeOffset = 0, k = 0;
	    k < out->len;
	    ++sizeOffset) {
		for(aIter = 0; aIter < aIncr; ++aIter, ++k)
			out->elements[k] = a->elements[aIter + aIncr * sizeOffset];
      
		for(bIter = 0; bIter < bIncr; ++bIter, ++k)
			out->elements[k] = b->elements[bIter + bIncr * sizeOffset];
	}

	return out;
}


//dim is a number to specifiy along which direction to concatenate
//along, it starts from 0
err_ret cat_mat(struct matrix const *const a, struct matrix const *const b, uint8_t dim, struct matrix **out) {
	int aScalar = is_scalar(a);
	int bScalar = is_scalar(b);

	err_ret err = 0;
	uint16_t *size = NULL;
	uint16_t fixed_size[3];
	switch(aScalar + bScalar){

	case 0: //a and b are not scalars
		//a and b are the same dim
		if(a->dim != b->dim)
			break;

		uint16_t *sizeA = malloc(sizeof(*sizeA) * (a->dim + 1));
		if(!sizeA)
			return e_malloc;

		uint16_t *sizeB = malloc(sizeof(*sizeB) * (b->dim + 1));
		if(!sizeB)
			return e_malloc;

		sizeA[a->dim] = 0;
		sizeB[b->dim] = 0;

		//creates a new size matrix, that skips the dim that
		//is being concatenated against
		for(int i = 0, j = 0; i < ( a->dim - 1); ++i, ++j){
			if(i == dim)
				++i;

			sizeA[j] = a->size[i];
			sizeB[j] = b->size[i];
		}

		if(!cmp_size(sizeA, sizeB, a->dim - 1, b->dim - 1))
			return e_size_mismatch;

		size = malloc(sizeof(*size) * (a->dim + 1));
		if(!size)
			return e_malloc;

		size[a->dim] = 0;
			
		for(int i = 0; i < a->dim; ++i){
			size[i] = a->size[i];
			if(i == dim)
				size[i] += b->size[i];
		}


		if((err = init_mat(size, a->dim, out)))
			return err;
		free(size);

		assignConcat(*out, a, b, dim);

		free(sizeA);
		free(sizeB);
		break;

	case 1:; //only one of a or b are scalars
		//temporary variables for less writing in if blocks
		struct matrix const *tempVector = NULL;
		ele tempScalar = 0;
		//assign which matrix is a scalar and which is a matrix
		if(aScalar) {
			tempVector = b;
			tempScalar = a->elements[0];

		} else {
			tempVector = a;
			tempScalar = b->elements[0];

		}

		//check that the matrix is a vector, only vectors can be
		//concatenated with scalars
		if(!is_vec(tempVector))
			return e_size_mismatch;

		//new size vector
		size = fixed_size;
		memcpy(size, tempVector->size, 3 * sizeof(*size));

		//increment size because of the concatenation
		++size[dim];

		//init new matrix
		if((err = init_mat(size, tempVector->dim, out)))
			return err;

		//put values into new matrix
		//first vector values
		for(uint64_t i = 0; i < tempVector->len; ++i)
			out[0]->elements[i + aScalar] = tempVector->elements[i];

		//then scalar value
		//assume that bScalar is either 0 or 1, this then puts
		//the scalar value at either the beginning or the end
		out[0]->elements[tempVector->len * bScalar] = tempScalar;
		break;

	case 2: //both a and b are scalars
		//create and set new size vector
		size = fixed_size;

		size[0] = 1;
		size[1] = 1;
		size[2] = 0;

		if(dim == 0){
			++size[0]; //set newSize to [2, 1]
		} else if(dim == 1){
			++size[1]; //set newSize to [1, 2]
		} else{
			return e_invalid_assign;
		}

		if((err = init_mat(size, 2, out)))
			return err;

		out[0]->elements[0] = a->elements[0];
		out[0]->elements[1] = b->elements[0];
		break;

	default: return e_fatal; //return error if something else
	}

	return err;
}


//free the matrix and all of the data
//associated with it
void free_mat(struct matrix *m) {
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
void print_two_d(const struct matrix *m, int offset) {

	puts("");
	for(int columns = 0; columns < m->size[0]; ++columns) {
		for(int rows = 0; rows < m->size[1]; ++rows) {
			//the below is the same as sub2ind for a 2d matrix
			//columns + m.size[0] * rows
			printf("%lf ", m->elements[offset + (columns + m->size[0] * rows)]);
		}
		puts("");
	}
	puts("");

	return;
}


//print out a matrix of any size
//prints out 2d slices of the matrix
void print_mat(const struct matrix *m) {
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
err_ret is_scalar(struct matrix const *const m) {
	return (m->dim == 1);
}

err_ret is_vec(struct matrix const *const m) {
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
err_ret chk_inner(struct matrix *a, struct matrix *b){
	return (a->size[a->dim - 1] == b->size[0]);
}
