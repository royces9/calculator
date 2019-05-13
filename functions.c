#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "operatorUtility.h"
#include "functions.h"


matrix *eye(matrix *a, err_ret *error) {
	if(a->dim != 1) {
		*error = -12;
		return NULL;
	} else if(a->elements[0] < 1) {
		*error = -4;
		return NULL;
	}

	uint16_t newSize[3] = {a->elements[0], a->elements[0], 0};

	matrix *out = init_mat(newSize, 2, error);
	if(*error)
		return NULL;

	for(uint64_t i = 0; i < a->elements[0]; ++i)
		out->elements[i * (uint64_t)(a->elements[0] + 1)] = 1;

	return out;
}


matrix *get_size(matrix *a, err_ret *error){
	uint16_t newSize[3];

	//output is a row vector
	newSize[0] = 1;
	newSize[1] = a->dim;
	newSize[2] = 0;
  
	matrix *out = init_mat(newSize, 2, error);
	if(*error)
		return NULL;

	for(uint64_t i = 0; a->size[i]; ++i)
		out->elements[i] = a->size[i];

	return out;
}


matrix *magnitude(matrix *a, err_ret *error){
	matrix *out = NULL;

	if(is_vec(a)) {
		ele mag_a = 0;

		for(uint64_t i = 0; i < a->len; ++i)
			mag_a += (a->elements[i] * a->elements[i]);

		mag_a = sqrt(mag_a);

		out = init_scalar(mag_a);
		__MALLOC_CHECK(out, *error);
	} else {
		*error = -10;
	}

	return out;
}


//get the total number of elements of a
matrix *numel(matrix *a) {
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
matrix *reference(matrix *a, matrix *b, err_ret *error) {
	matrix *out = cpy_mat(b);
	if( !out )
		return NULL;

	for(uint64_t i = 0; i < b->len; ++i)
		out->elements[i] = a->elements[(uint64_t) (b->elements[i])];

	return out;
}


matrix *assign(matrix *a, matrix *b, vari *var, err_ret *error) {

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
			uint64_t index = var->assign->elements[i];
			if(!index) {
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


matrix *div_mat(matrix *a, matrix *b, err_ret *error) {
	return NULL;
}


matrix *mult_mat(matrix *a, matrix *b, err_ret *error) {
	matrix *out = NULL;
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

	matrix *t_a = t_mat(a, error);

	//generic O(n^3) algorithm
	//transpose a and then multiply every column with every other column in each matrix
	for(uint16_t i = 0, l = 0; i < b->size[1]; ++i){
		for(uint16_t j = 0; j < t_a->size[1]; ++j){
			ele tmp = 0;

			for(uint16_t k = 0; k < t_a->size[0]; ++k) {
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


matrix *exp_mat(matrix *a, matrix *b, err_ret *error) {
	uint8_t aScalar = is_scalar(a);
	uint8_t bScalar = is_scalar(b);

	matrix *tmp = NULL;
	matrix *out = NULL;

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
			if((b->elements[0] - floor(b->elements[0])) < 0.00000000001) {
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
		}
		break;


	case 2: //a and b are both scalar
		out = init_scalar(pow(a->elements[0], b->elements[0]));
		__MALLOC_CHECK(out, *error);
		break;

	default: *error = -10; break;
	}

	return out;
}


matrix *t_mat(matrix *a, err_ret *error) {
	//transpose only defined for 2d matrix
	if(a->dim != 2) {
		*error = -10;
		return NULL;
	}

	//new transposed size is same as a->size
	//but the dimensions are swapped
	uint16_t newSize[3] = {a->size[1], a->size[0], 0};
	matrix *out = init_mat(newSize, 2, error);
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
matrix *min(matrix *m, err_ret *error) {
	ele out = m->elements[0];

	for(uint64_t i = 1; i < m->len; ++i)
		out = fmin(out, m->elements[i]);

	return init_scalar(out);
}


//determines maximum value in the matrix
matrix *max(matrix *m, err_ret *error) {
	ele out = m->elements[0];

	for(uint64_t i = 1; i < m->len; ++i)
		out = fmax(out, m->elements[i]);

	return init_scalar(out);
}


//sums along the last dimension of the matrix
//unless matrix, then sum the elements
matrix *sum(matrix *m, err_ret *error) {
	matrix *out = NULL;

	int new_dim = m->dim - 1;
	uint16_t *newSize = NULL;

	if(is_vec(m)) {
		newSize = malloc(sizeof(*newSize) * (new_dim + 1));
		__MALLOC_CHECK(newSize, *error);

		newSize[0] = 1;
		newSize[1] = 0;

	} else if(m->dim == 2) {
		new_dim = 2;
		newSize = malloc(sizeof(*newSize) * (new_dim + 1));
		__MALLOC_CHECK(newSize, *error);
		
		newSize[0] = 1;
		newSize[1] = m->size[1];
		newSize[2] = 0;
 
	} else if(m->dim == 1) {
		return cpy_mat(m);

	} else {
		newSize = malloc(sizeof(*newSize) * (new_dim + 1));
		__MALLOC_CHECK(newSize, *error);

		newSize = memcpy(newSize, m->size,
				 sizeof(*newSize) * (new_dim + 1));
		newSize[new_dim] = 0;
	}

	out = init_mat(newSize, new_dim, error);
	free(newSize);
	if(*error)
		return NULL;

	for(uint64_t i = 0; i < out->len; ++i){
		for(uint16_t j = 0; j < m->size[m->dim - 2]; ++j){
			uint64_t ind = i * m->size[m->dim - 2] + j;
			out->elements[i] += m->elements[ind];
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
matrix *avg(matrix *m, err_ret *error) {
	matrix *out = sum(m, error);

	for(uint64_t i = 0; i < out->len; ++i)
		out->elements[i] /= m->size[m->dim - 1];

	return out;
}


ele _sin(ele a, err_ret *e) {
	return sin(a);
}

ele _cos(ele a, err_ret *e) {
	return cos(a);
}

ele _tan(ele a, err_ret *e) {
	return tan(a);
}

ele _log(ele a, err_ret *e) {
	return log(a);
}

ele _log10(ele a, err_ret *e) {
	return log10(a);
}

ele _sqrt(ele a, err_ret *e) {
	return sqrt(a);
}

ele _asin(ele a, err_ret *e) {
	return asin(a);
}

ele _acos(ele a, err_ret *e) {
	return acos(a);
}

ele _atan(ele a, err_ret *e) {
	return atan(a);
}

ele _floor(ele a, err_ret *e) {
	return floor(a);
}

ele _ceil(ele a, err_ret *e) {
	return ceil(a);
}

ele _round(ele a, err_ret *e) {
	return round(a);
}

ele factorial(ele a, err_ret *e) {
	a = floor(a);

	return a < 2 ? 1 : a * factorial(a - 1, e);
}


//returns value from one argument functions
ele one_arg(ele a, int o, err_ret *error) {
	switch(o) {
	case eSin: return sin(a);
	case eCos: return cos(a);
	case eTan: return tan(a);
	case eLn: return log(a);
	case eLog: return log10(a);
	case eSqrt: return sqrt(a);
	case eAsin: return asin(a);
	case eAcos: return acos(a);
	case eAtan: return atan(a);
	case eFloor: return floor(a);
	case eCeil: return ceil(a);
	case eRound: return round(a);
	case eFactorial: return factorial(a, error);
	default: *error = 1; return a;
	}
}


//returns value from two argument function
ele two_arg(ele a, ele b, int o, err_ret *error) {
	switch(o) {
	case eAdd: return a + b;
	case eSubtract: return a - b;
	case eMultiply: return a * b;
	case eDivide: return a / b;
	case eExponent: return pow(a, b);
	case eLess: return a < b;
	case eGreater: return a > b;
	case eLessEqual: return a <= b;
	case eGreaterEqual: return a >= b;
	case eNotEqual: return a != b;
	case eEqual: return a == b;
	case eAnd: return a && b;
	case eOr: return a || b;
	case eModulo: return (int64_t) a % (int64_t) b;
	default: *error = 1; return a;
	}
}
