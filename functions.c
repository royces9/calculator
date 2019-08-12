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
		return e_invalid_expr;
	} else if(a->elements[0] < 1) {
		return e_invalid_expr;
	}

	int newSize[3] = {a->elements[0], a->elements[0], 0};

	err = init_mat(newSize, 2, out);
	if(err)
		return err;

	for(uint64_t i = 0; i < a->elements[0]; ++i)
		out[0]->elements[i * (uint64_t)(a->elements[0] + 1)] = 1;

	return err;
}


err_ret get_size(struct matrix const *const a, struct matrix **out) {
	err_ret err = 0;
	int newSize[3];

	//output is a row vector
	newSize[0] = 1;
	newSize[1] = a->dim;
	newSize[2] = 0;
  

	err = init_mat(newSize, 2, out);
	if(err)
		return err;

	for(uint64_t i = 0; a->size[i]; ++i)
		out[0]->elements[i] = a->size[i];

	return err;
}


err_ret magnitude(struct matrix const *const a, struct matrix **out) {
	if(!is_vec(a))
		return e_size_mismatch;

	ele mag_a = 0;

	for(long i = 0; i < a->len; ++i)
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

	for(long i = 0; i < b->len; ++i) {
		long ind = b->elements[i];
		if((ind < 0) || (ind > out[0]->len) ) {
			err = e_bound;
			break;
		}
		out[0]->elements[i] = a->elements[ind];
	}

	return err;
}


err_ret assign(struct matrix *a, struct matrix *b, struct vari *var, struct matrix **out) {
	uint8_t incrementFlag = 1;

	if(!a->var) {
		free_mat(a);
		return e_ok;
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
			return e_malloc;

		memcpy(a->elements, b->elements, a->len * sizeof(*a->elements));

		a->size = malloc((a->dim + 1) * sizeof(*a->size));
		if(!a->size)
			return e_malloc;

		memcpy(a->size, b->size, (a->dim + 1) * sizeof(*a->size));

	} else {
		for(uint64_t i = 0, k = 0, *j = is_scalar(b) ? &k : &i;
		    i < var->assign->len;
		    ++i) {
			int index = var->assign->elements[i];
			if(index < 0)
				return e_invalid_assign;

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
		return e_size_mismatch;

	//check that the inner dimensions match
	if(a->size[1] != b->size[0])
		return e_size_mismatch;

	err_ret err = 0;
	int newSize[3] = {a->size[0], b->size[1], 0};
	
	err = init_mat(newSize, 2, out);
	if(err)
		return err;

	struct matrix *t_a = NULL;
	err = t_mat(a, &t_a);
	if(err)
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
		return e_size_mismatch;

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
				return e_size_mismatch;
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
		err = e_size_mismatch;
		break;
	}

	return err;
}


err_ret t_mat(struct matrix const *const a, struct matrix **out) {
	//transpose only defined for 2d matrix
	if(a->dim != 2)
		return e_size_mismatch;

	//new transposed size is same as a->size
	//but the dimensions are swapped
	int newSize[3] = {a->size[1], a->size[0], 0};
	err_ret err = init_mat(newSize, 2, out);
	if(err)
		return err;

	for(long i = 0; i < out[0]->len; ++i) {
		//tmp is an int and gets rounded down
		long tmp = i / a->size[0];
		long ind = tmp + a->size[1] * (i - tmp * a->size[0]);

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
		int new_size[] = {1, m->size[1], 0};

		if((err = init_mat(new_size, 2, out)))
			return err;

		for(int i = 0; i < out[0]->len; ++i) {
			for(int j = 0; j < m->size[m->dim - 2]; ++j) {
				long ind = i * m->size[m->dim - 2] + j;
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
				long ind = j * out[0]->len + i;
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

	for(long i = 0; i < out[0]->len; ++i)
		out[0]->elements[i] /= m->size[m->dim - 1];

	return err;
}

void __assign_mat_missing_col(struct matrix const *const a, struct matrix *b, int col) {
	
	for(int aa = 0, bb = 0; aa < a->size[0]; ++aa, ++bb) {
		if(aa == col) {
			--bb;
			continue;
		}

		for(int i = 0; i < b->size[0]; ++i) {
			b->elements[i + bb * b->size[0]] = a->elements[(i + 1) + aa * a->size[0]];
		}
	}
}


//use recursive minor expansion formula
err_ret __recurse_det(struct matrix const *const m, ele *out) {
	struct matrix *tmp_mat = NULL;
	int tmpsize[2] = {m->size[0] - 1, m->size[0] - 1};
	err_ret err = init_mat(tmpsize, 2, &tmp_mat);
	if(err)
		return err;

	int sign[2] = {1, -1};
	for(int i = 0, ind = 0; i < m->size[0]; ++i, ind = !ind) {
		ele ans = 0;
		__assign_mat_missing_col(m, tmp_mat, i);
		
		err = __recurse_det((struct matrix const *const)tmp_mat, &ans);
		if(err)
			break;
		*out += ans * sign[ind] * m->elements[i * m->size[0]];
	}

	free_mat(tmp_mat);
	return err;
}


/*
err_ret __efficient_det(struct matrix const *const m, ele *out) {
	int sub_size = m->size[0] - 2;
	int *ind = malloc(sub_size * sizeof(*ind));
	
	for(int i = 0; i < sub_size; ++i)
		ind[i] = i;
	
	int size = m->size;
	int sign[2] = {1, -1};
	int ind = 0;
	
	for(int ii = 0; i < (size - 2); ++i, ind = !ind) {
	}

	return 0;
}
*/

err_ret determinant(struct matrix const *const m, struct matrix **out) {
	if(is_scalar(m))
		return init_scalar(m->elements[0], out);
	
	if(m->dim != 2)
		return e_size_mismatch;
	
	if(m->size[0] != m->size[1])
		return e_size_mismatch;

	ele det = 0;
	ele err = 0;
	if(m->size[0] == 3) {
		det = m->elements[0] * m->elements[4] * m->elements[8]
			+ m->elements[3] * m->elements[7] * m->elements[2]
			+ m->elements[6] * m->elements[1] * m->elements[5]
			- m->elements[6] * m->elements[4] * m->elements[2]
			- m->elements[0] * m->elements[7] * m->elements[5]
			- m->elements[3] * m->elements[1] * m->elements[8];
	} else if(m->size[0] == 2) {
		det = m->elements[0] * m->elements[3]
			- m->elements[1] * m->elements[2];
	} else {
		//err = __efficient_det(m, &det);
		err = __recurse_det(m, &det);
		if(err)
			return err;
	}

	err = init_scalar(det, out);
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

