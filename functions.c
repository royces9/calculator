#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "operatorUtility.h"
#include "functions.h"


struct matrix *eye(struct matrix *a, err_ret *error) {
	if(a->dim != 1) {
		*error = -12;
		return NULL;
	} else if(a->elements[0] < 1) {
		*error = -4;
		return NULL;
	}

	uint16_t newSize[3] = {a->elements[0], a->elements[0], 0};

	struct matrix *out = init_mat(newSize, 2, error);
	if(*error)
		return NULL;

	for(uint64_t i = 0; i < a->elements[0]; ++i)
		out->elements[i * (uint64_t)(a->elements[0] + 1)] = 1;

	return out;
}


struct matrix *get_size(struct matrix *a, err_ret *error) {
	uint16_t newSize[3];

	//output is a row vector
	newSize[0] = 1;
	newSize[1] = a->dim;
	newSize[2] = 0;
  
	struct matrix *out = init_mat(newSize, 2, error);
	if(*error)
		return NULL;

	for(uint64_t i = 0; a->size[i]; ++i)
		out->elements[i] = a->size[i];

	return out;
}


struct matrix *magnitude(struct matrix *a, err_ret *error) {
	if(!is_vec(a)) {
		*error = -10;
		return NULL;
	}

	struct matrix *out = NULL;
	ele mag_a = 0;

	for(uint64_t i = 0; i < a->len; ++i)
		mag_a += (a->elements[i] * a->elements[i]);

	mag_a = sqrt(mag_a);

	out = init_scalar(mag_a);
	__MALLOC_CHECK(out, *error);

	return out;
}


//get the total number of elements of a
struct matrix *numel(struct matrix *a, err_ret *error) {
	return init_scalar(a->len);
}


/*function for matrix referencing
 *  ex:
 *  a = linspace(0,1,3)
 *  0, 0.5, 1
 *  b = a(2) + 1
 *  1.5
 *
 *  a - the matrix
 *  b - the index
 */
struct matrix *reference(struct matrix *a, struct matrix *b, err_ret *error) {
	struct matrix *out = cpy_mat(b);

	if( !out ) {
		*error = -6;
		return NULL;
	}

	for(uint64_t i = 0; i < b->len; ++i)
		out->elements[i] = a->elements[(uint64_t) (b->elements[i])];

	return out;
}


struct matrix *assign(struct matrix *a, struct matrix *b, struct vari *var, err_ret *error) {

	uint8_t incrementFlag = 1;

	if(!a->var) {
		*error = -13;
		free_mat(a);
		return NULL;

	}

	if(!var->assign) {
		//init new matrix
		//copyMatrix not done because the
		//pointer 'a' is malloc'd in find_fun
		if(a->size) {
			free(a->size);
			free(a->elements);

			a->size = NULL;
			a->elements = NULL;

			incrementFlag = 0;
		}
      
		a->len = b->len;
		a->dim = b->dim;

		a->elements = malloc(sizeof(*a->elements) * a->len);
		__MALLOC_CHECK(a->elements, *error);

		memcpy(a->elements, b->elements, sizeof(*a->elements) * a->len);

		a->size = malloc(sizeof(*a->size) * (a->dim + 1));
		__MALLOC_CHECK(a->size, *error);

		memcpy(a->size, b->size, sizeof(*a->size) * (a->dim + 1));

	} else {
		for(uint64_t i = 0, k = 0, *j = is_scalar(b) ? &k : &i;
		    i < var->assign->len;
		    ++i) {
			int index = var->assign->elements[i];
			if(index < 0) {
				*error = -13;
				break;
			}
			a->elements[index] = b->elements[*j];
		} 

		incrementFlag = 0;
		free_mat(var->assign);
		var->assign = NULL;

	}


	if(incrementFlag)
		++var->count;

	return cpy_mat(a);
}


struct matrix *div_mat(struct matrix *a, struct matrix *b, err_ret *error) {
	return NULL;
}


struct matrix *mult_mat(struct matrix *a, struct matrix *b, err_ret *error) {
	struct matrix *out = NULL;
	//matrix multiplication only defined for 2d arrays
	if((a->dim != 2) || (b->dim != 2)) {
		*error = -10;
		return NULL;
	}

	//check that the inner dimensions match
	if(a->size[1] != b->size[0]) {
		*error = -10;
		return NULL;
	}

	uint16_t newSize[3] = {a->size[0], b->size[1], 0};
	out = init_mat(newSize, 2, error);
	if(*error)
		return NULL;

	struct matrix *t_a = t_mat(a, error);

	//generic O(n^3) algorithm
	//transpose a and then multiply every column
	//with every other column in each matrix
	for(int i = 0, l = 0; i < b->size[1]; ++i){
		for(int j = 0; j < t_a->size[1]; ++j){
			ele tmp = 0;

			for(int k = 0; k < t_a->size[0]; ++k) {
				uint64_t ind_a = k + j * t_a->size[0];
				uint64_t ind_b = k + i * b->size[0];
				tmp += t_a->elements[ind_a] * b->elements[ind_b];
			}

			out->elements[l] = tmp;
			++l;
		}
	}
    
	free_mat(t_a);
	return out;
}


struct matrix *exp_mat(struct matrix *a, struct matrix *b, err_ret *error) {
	uint8_t aScalar = is_scalar(a);
	uint8_t bScalar = is_scalar(b);

	struct matrix *tmp = NULL;
	struct matrix *out = NULL;

	switch(aScalar + bScalar) {
	case 0: //neither a nor b are scalars
		*error = -10;
		break;

	case 1: //one of a or b is a scalar
		if(aScalar) { //a is the scalar
			if( !(out = cpy_mat(b)) ) {
				*error = -6;
				break;
			}

			for(uint64_t i = 0; i < out->len; ++i)
				out->elements[i] = pow(a->elements[0],b->elements[i]);

		} else { //b is the scalar
			//check that b is a whole number, no imaginary numbers (yet?)

			out = init_scalar(a->size[0]);
			__MALLOC_CHECK(out, *error);

			tmp = eye(out, error);
			free_mat(out);

			//really small number
			if((b->elements[0] - floor(b->elements[0])) > 0.00000000001) {
				*error = -10;
				free_mat(tmp);
				break;
			}

			int64_t power = b->elements[0];
			for(int64_t i = 0; i < power; ++i) {
				out = mult_mat(tmp, a, error);
				free_mat(tmp);

				if(*error) {
					free_mat(out);
					return NULL;
				}

				tmp = cpy_mat(out);
				if( !tmp ) {
					*error = -6;
					return NULL;
				}
				free_mat(out);
			}
			out = tmp;
		}
		break;


	case 2: //a and b are both scalar
		out = init_scalar(pow(a->elements[0], b->elements[0]));
		__MALLOC_CHECK(out, *error);
		break;

	default:
		*error = -10;
		break;
	}

	return out;
}


struct matrix *t_mat(struct matrix *a, err_ret *error) {
	//transpose only defined for 2d matrix
	if(a->dim != 2) {
		*error = -10;
		return NULL;
	}

	//new transposed size is same as a->size
	//but the dimensions are swapped
	uint16_t newSize[3] = {a->size[1], a->size[0], 0};
	struct matrix *out = init_mat(newSize, 2, error);
	if(*error)
		return NULL;

	for(uint64_t i = 0; i < out->len; ++i) {
		//tmp is an int and gets rounded down
		uint64_t tmp = i / a->size[0];
		uint64_t ind = tmp + a->size[1] * (i - tmp * a->size[0]);

		out->elements[ind] = a->elements[i];
	}

	return out;
}


//determinies minimum value in the matrix
struct matrix *min(struct matrix *m, err_ret *error) {
	ele out = m->elements[0];

	for(uint64_t i = 1; i < m->len; ++i)
		out = fmin(out, m->elements[i]);

	return init_scalar(out);
}


//determines maximum value in the matrix
struct matrix *max(struct matrix *m, err_ret *error) {
	ele out = m->elements[0];

	for(uint64_t i = 1; i < m->len; ++i)
		out = fmax(out, m->elements[i]);

	return init_scalar(out);
}


//sums along the last dimension of the matrix
//unless matrix, then sum the elements
struct matrix *sum(struct matrix *m, err_ret *error) {
	struct matrix *out = NULL;

	if(is_vec(m)) {
		if( !(out = init_scalar(0))) {
			*error = -6;
			return NULL;
		}

		for(int i = 0; i < m->len; ++i)
			out->elements[0] += m->elements[i];

	} else if(m->dim == 2) {
		uint16_t new_size[] = {1, m->size[1], 0};

		out = init_mat(new_size, 2, error);
		if(*error)
			return out;

		for(int i = 0; i < out->len; ++i) {
			for(int j = 0; j < m->size[m->dim - 2]; ++j) {
				uint64_t ind = i * m->size[m->dim - 2] + j;
				out->elements[i] += m->elements[ind];
			}
		}
	} else if(m->dim == 1) {
		if( !(out = init_scalar(m->elements[0])) )
			*error = -6;
		return out;
	} else {
		out = init_mat(m->size, m->dim - 1, error);
		if(*error)
			return NULL;

		for(int i = 0; i < out->len; ++i) {
			for(int j = 0; j < m->size[m->dim - 1]; ++j) {
				uint64_t ind = j * out->len + i;
				out->elements[i] += m->elements[ind];
			}
		}
	}
	
	return out;
}

//calculates average value of the matrix along the last dimensions columns
//ex:
//1 2
//3 4
//should return
//2 3
struct matrix *avg(struct matrix *m, err_ret *error) {
	struct matrix *out = sum(m, error);

	for(uint64_t i = 0; i < out->len; ++i)
		out->elements[i] /= m->size[m->dim - 1];

	return out;
}


ele _sin(ele a) {
	return sin(a);
}

ele _cos(ele a) {
	return cos(a);
}

ele _tan(ele a) {
	return tan(a);
}

ele _log(ele a) {
	return log(a);
}

ele _log10(ele a) {
	return log10(a);
}

ele _sqrt(ele a) {
	return sqrt(a);
}

ele _asin(ele a) {
	return asin(a);
}

ele _acos(ele a) {
	return acos(a);
}

ele _atan(ele a) {
	return atan(a);
}

ele _floor(ele a) {
	return floor(a);
}

ele _ceil(ele a) {
	return ceil(a);
}

ele _round(ele a) {
	return round(a);
}

ele factorial(ele a) {
	a = floor(a);

	return a < 2 ? 1 : a * factorial(a - 1);
}

