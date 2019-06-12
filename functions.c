#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "operatorUtility.h"
#include "functions.h"


err_ret eye(struct matrix const *const a, struct matrix **out) {
	err_ret err = 0;
	if(a->dim != 1) {
		return -12;
	} else if(a->elements[0] < 1) {
		return -4;
	}

	uint16_t newSize[3] = {a->elements[0], a->elements[0], 0};

	if((err = init_mat(newSize, 2, out)))
		return err;

	for(uint64_t i = 0; i < a->elements[0]; ++i)
		out[0]->elements[i * (uint64_t)(a->elements[0] + 1)] = 1;

	return err;
}


err_ret get_size(struct matrix const *const a, struct matrix **out) {
	err_ret err = 0;
	uint16_t newSize[3];

	//output is a row vector
	newSize[0] = 1;
	newSize[1] = a->dim;
	newSize[2] = 0;
  

	if((err = init_mat(newSize, 2, out)))
		return err;

	for(uint64_t i = 0; a->size[i]; ++i)
		out[0]->elements[i] = a->size[i];

	return err;
}


err_ret magnitude(struct matrix const *const a, struct matrix **out) {
	if(!is_vec(a))
		return -10;

	ele mag_a = 0;

	for(uint64_t i = 0; i < a->len; ++i)
		mag_a += (a->elements[i] * a->elements[i]);

	mag_a = sqrt(mag_a);

	return init_scalar(mag_a, out);
}


//get the total number of elements of a
err_ret numel(struct matrix const *const a, struct matrix **out) {
	return init_scalar(a->len, out);
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
err_ret reference(struct matrix const *const a, struct matrix const *const b, struct matrix **out) {
	err_ret err = cpy_mat(b, out);
	if(err)
		return err;

	for(uint64_t i = 0; i < b->len; ++i)
		out[0]->elements[i] = a->elements[(uint64_t) (b->elements[i])];

	return err;
}


err_ret assign(struct matrix *a, struct matrix *b, struct vari *var, struct matrix **out) {
	uint8_t incrementFlag = 1;

	if(!a->var) {
		free_mat(a);
		return 0;
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

		a->elements = malloc(a->len * sizeof(*a->elements));
		if(!a->elements)
			return -6;

		memcpy(a->elements, b->elements, a->len * sizeof(*a->elements));

		a->size = malloc((a->dim + 1) * sizeof(*a->size));
		if(!a->size)
			return -6;

		memcpy(a->size, b->size, (a->dim + 1) * sizeof(*a->size));

	} else {
		for(uint64_t i = 0, k = 0, *j = is_scalar(b) ? &k : &i;
		    i < var->assign->len;
		    ++i) {
			int index = var->assign->elements[i];
			if(index < 0)
				return -13;

			a->elements[index] = b->elements[*j];
		} 

		incrementFlag = 0;
		free_mat(var->assign);
		var->assign = NULL;

	}


	if(incrementFlag)
		++var->count;

	return cpy_mat(a, out);
}


err_ret div_mat(struct matrix const *const a, struct matrix const *const b, struct matrix **out) {
	return 0;
}


err_ret mult_mat(struct matrix const *const a, struct matrix const *const b, struct matrix **out) {
	//matrix multiplication only defined for 2d arrays
	if((a->dim != 2) || (b->dim != 2))
		return -10;

	//check that the inner dimensions match
	if(a->size[1] != b->size[0])
		return -10;

	err_ret err = 0;
	uint16_t newSize[3] = {a->size[0], b->size[1], 0};
	if((err = init_mat(newSize, 2, out)))
		return err;

	struct matrix *t_a = NULL;
	if((err = t_mat(a, &t_a)))
		return err;

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

			out[0]->elements[l] = tmp;
			++l;
		}
	}
    
	free_mat(t_a);
	return err;
}


err_ret exp_mat(struct matrix const *const a, struct matrix const *const b, struct matrix **out) {
	uint8_t aScalar = is_scalar(a);
	uint8_t bScalar = is_scalar(b);

	struct matrix *tmp = NULL;

	err_ret err = 0;
	switch(aScalar + bScalar) {
	case 0: //neither a nor b are scalars
		return -10;

	case 1: //one of a or b is a scalar
		if(aScalar) { //a is the scalar
			if((err = cpy_mat(b, out)))
				return err;

			for(uint64_t i = 0; i < out[0]->len; ++i)
				out[0]->elements[i] = pow(a->elements[0],b->elements[i]);

		} else { //b is the scalar
			//check that b is a whole number, no imaginary numbers (yet?)

			if((err = init_scalar(a->size[0], out)))
				return err;

			err = eye(*out, &tmp);
			free_mat(*out);
			if(err)
				return err;

			//really small number
			if((b->elements[0] - floor(b->elements[0])) > 0.00000000001) {
				free_mat(tmp);
				return -10;
			}

			int64_t power = b->elements[0];
			for(int64_t i = 0; i < power; ++i) {
				err = mult_mat(tmp, a, out);
				free_mat(tmp);
				if(err) {
					free_mat(*out);
					return err;
				}
					
				if((err = cpy_mat(*out, &tmp)))
					return err;

				free_mat(*out);
			}
			*out = tmp;
		}
		break;


	case 2: //a and b are both scalar
		err = init_scalar(pow(a->elements[0], b->elements[0]), out);
		break;

	default:
		err = -10;
		break;
	}

	return err;
}


err_ret t_mat(struct matrix const *const a, struct matrix **out) {
	//transpose only defined for 2d matrix
	if(a->dim != 2)
		return -10;

	//new transposed size is same as a->size
	//but the dimensions are swapped
	uint16_t newSize[3] = {a->size[1], a->size[0], 0};
	err_ret err = init_mat(newSize, 2, out);
	if(err)
		return err;

	for(uint64_t i = 0; i < out[0]->len; ++i) {
		//tmp is an int and gets rounded down
		uint64_t tmp = i / a->size[0];
		uint64_t ind = tmp + a->size[1] * (i - tmp * a->size[0]);

		out[0]->elements[ind] = a->elements[i];
	}

	return err;
}


//determinies minimum value in the matrix
err_ret min(struct matrix const *const m, struct matrix **out) {
	ele cur_min = m->elements[0];

	for(uint64_t i = 1; i < m->len; ++i)
		cur_min = fmin(cur_min, m->elements[i]);

	return init_scalar(cur_min, out);
}


//determines maximum value in the matrix
err_ret max(struct matrix const *const m, struct matrix **out) {
	ele cur_max = m->elements[0];

	for(uint64_t i = 1; i < m->len; ++i)
		cur_max = fmax(cur_max, m->elements[i]);

	return init_scalar(cur_max, out);
}


//sums along the last dimension of the matrix
//unless matrix, then sum the elements
err_ret sum(struct matrix const *const m, struct matrix **out) {
	err_ret err = 0;
	if(is_vec(m)) {
		if((err = init_scalar(0, out)))
			return err;

		for(int i = 0; i < m->len; ++i)
			out[0]->elements[0] += m->elements[i];

	} else if(m->dim == 2) {
		uint16_t new_size[] = {1, m->size[1], 0};

		if((err = init_mat(new_size, 2, out)))
			return err;

		for(int i = 0; i < out[0]->len; ++i) {
			for(int j = 0; j < m->size[m->dim - 2]; ++j) {
				uint64_t ind = i * m->size[m->dim - 2] + j;
				out[0]->elements[i] += m->elements[ind];
			}
		}
	} else if(m->dim == 1) {
		err = init_scalar(m->elements[0], out);
	} else {
		if((err = init_mat(m->size, m->dim - 1, out)))
			return err;

		for(int i = 0; i < out[0]->len; ++i) {
			for(int j = 0; j < m->size[m->dim - 1]; ++j) {
				uint64_t ind = j * out[0]->len + i;
				out[0]->elements[i] += m->elements[ind];
			}
		}
	}

	return err;
}

//calculates average value of the matrix along the last dimensions columns
//ex:
//1 2
//3 4
//should return
//2 3
err_ret avg(struct matrix const *const m, struct matrix **out) {
	err_ret err = sum(m, out);
	if(err)
		return err;

	for(uint64_t i = 0; i < out[0]->len; ++i)
		out[0]->elements[i] /= m->size[m->dim - 1];

	return err;
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

