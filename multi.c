#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"

#include "sya.h"
#include "operatorUtility.h"
#include "multi.h"
#include "userFunctions.h"

int numberOfArgs(char **inp) {
	int i = 0;
	for(; inp[i]; ++i); //empty for
	return i;
}


err_ret deri(char **inp, struct vari *var, struct matrix **out) {
	//copy global struct to a local variable struct
	struct vari *tmp = NULL;
	err_ret err = cpy_var(var, &tmp);
	if(err)
		return err;

	//check the number of inps is correct
	if(numberOfArgs(inp) != 4) {
		err = -2;
		goto err_ret;
	}

	//set both the point and step size
	err = sya(inp[2], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele point = tmp->ans->elements[0];


	err = sya(inp[3], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele h = tmp->ans->elements[0];

  
	//set up a dummy variable specified by user  
	char *tmp_var = removeSpaces(inp[1]);
	struct matrix *temp = NULL;
	err = init_scalar(point + h, &temp);
	if(err)
		goto err_ret;

	int var_ind = set_var(tmp, tmp_var, temp, &err);
	if(err)
		goto err_ret;

	//f(x+h)
	err = sya(inp[0], tmp);
	if(err)
		goto err_ret;

	ele ans = tmp->ans->elements[0];

	//sets the dummy variable equal to x-h
	tmp->value[var_ind]->elements[0] = point - h;

	//f(x-h)
	err = sya(inp[0], tmp);
	if(err)
		goto err_ret;

	//f(x+h) - f(x-h)
	ans -= tmp->ans->elements[0];

	err = init_scalar(ans / (2 * h), out);

 err_ret:
	free_var(tmp);

	return err;
}


err_ret inte(char **inp, struct vari *var, struct matrix **out) {
	//check number of arguments
	if(numberOfArgs(inp) != 5)
		return -2;

	//copy global struct to a local variable struct
	struct vari *tmp = NULL;
	err_ret err = cpy_var(var, &tmp);
	if(err)
		return err;

	int var_ind = 0;

	//get number of steps, and step size
	err = sya(inp[2], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele a = tmp->ans->elements[0];


	err = sya(inp[3], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele b = tmp->ans->elements[0];


	err = sya(inp[4], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele number = tmp->ans->elements[0];

	//calculate step size
	ele step = (b - a) / number;

	//set dummy variable
	char *tmp_var = removeSpaces(inp[1]);


	struct matrix *temp = NULL;
	err = init_scalar(0, &temp);
	if(err)
		goto err_ret;
		
	var_ind = set_var(tmp, tmp_var, temp, &err);
	//calculate integral using composite Simpson's

	number = floor(number/2); //halve the steps

	ele sum = 0;
	for(int i = 1; i <= number; ++i) {
		//f(x_2i-2)
		tmp->value[var_ind]->elements[0] = a + (((2 * i) - 2) * step);
		err = sya(inp[0], tmp);
		if(err)
			goto err_ret;
		sum += tmp->ans->elements[0];


		//4*f(x_2i-1)
		tmp->value[var_ind]->elements[0] = a + (((2 * i) - 1) * step);
		err = sya(inp[0], tmp);
		if(err)
			goto err_ret;
		sum += (4 * tmp->ans->elements[0]);


		//f(x_2i)
		tmp->value[var_ind]->elements[0] = a + ((2 * i) * step);
		err = sya(inp[0], tmp);
		if(err)
			goto err_ret;
		sum += tmp->ans->elements[0];
	}


	err = init_scalar(sum * (step / 3), out);

 err_ret:
	free_var(tmp);

	//return integral
	return err;
}


err_ret solve(char **inp, struct vari *var, struct matrix **out) {
	//check number of arguments
	if(numberOfArgs(inp) != 4)
		return -2;
	
	struct vari *tmp = NULL;
	//copy global struct to a local variable struct
	err_ret err =  cpy_var(var, &tmp);
	if(err)
		return err;
  
	double const delta = 0.000001;

	/*
	  inp[0] = function
	  inp[1] = variable
	  inp[2] = initial guess
	  inp[3] = tolerance
	*/

	//set dummy variable

	//set initial guess and the tolerance

	err = sya(inp[2], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}

	char *tmp_var = removeSpaces(inp[1]);
	struct matrix *cpy = NULL;
	err = cpy_mat(tmp->ans, &cpy);
	if(err)
		goto err_ret;

	int var_ind = set_var(tmp, tmp_var, cpy, &err);

	err = sya(inp[3], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele h = tmp->ans->elements[0];

	//ensure test is always greater than h
	//on start
	ele test = h + 1;

	//counter to keep track of the amount of iterations
	//if it overflows, then break from the loop
	uint16_t counter = 1;

	//solve f(x)=0 for x using Newton's method
	ele ans = 0;
	ele inter = 0;

	//if the difference between iterations is
	//less than the tolerance, break out of loop
	while(fabs(test) > h) {
		err = sya(inp[0], tmp);
		if(err)
			goto err_ret;

		ans = tmp->ans->elements[0];

		tmp->value[var_ind]->elements[0] -= delta;
		err = sya(inp[0], tmp);
		if(err)
			goto err_ret;

		inter = tmp->ans->elements[0];

		test = (delta * ans) / (ans - inter);
		tmp->value[var_ind]->elements[0] -= test;

		//if counter overflows and goes back to 0
		//this is true, max value is 65535 (2 bytes)
		++counter;
		if(!counter) {
			err = -12;
			goto err_ret;
		}
	}


	err = cpy_mat(tmp->value[var_ind], out);

 err_ret:
	free_var(tmp);

	return err;
}


err_ret zeros(char **inp, struct vari *var, struct matrix **out) {
	int dim = numberOfArgs(inp);
	uint16_t *size = NULL;

	err_ret err = 0;
	//only one inp, make a square matrix of that size
	//or if it's a matrix, make one of that size
	switch(dim) {
	case 1:
 		err = sya(inp[0], var);
		if(err)
			break;

		//check that the one inp is a scalar
		if(is_scalar(var->ans)) {
			//change dimension to make square matrix
			dim = 2;

			size = malloc((dim + 1) * sizeof(*size));
			if(!size) {
				err = -6;
				break;
			}

			if(!var->ans->elements[0]) {
				err = -11;
				break;
			}				

			size[0] = var->ans->elements[0];
			size[1] = var->ans->elements[0];
			size[2] = 0;

		} else if(is_vec(var->ans)) {
			dim = var->ans->dim;
			size = malloc((dim + 1) * sizeof(*size));
			if(!size) {
				err = -6;
				break;
			}
	
			uint8_t i = 0;
			for(; i < var->ans->len; ++i) {
				if(var->ans->elements[i]) {
					err = -11;
					break;
				}

				size[i] = var->ans->elements[i];
			}

			size[i] = 0;
		} else {
			err = -11;
		}
		break;

	default:
		size = malloc((dim + 1) * sizeof(*size));
		if(!size) {
			err = -6;
			break;
		}

		for(uint8_t i = 0; i < dim; ++i) {
			err = sya(inp[i], var);
			if(err)
				break;

			if( (var->ans->dim != 1) || !var->ans->elements[0]) {
				err = -10;
				break;
			}

			size[i] = var->ans->elements[0];
		}
	}

	if(!err) {
		size[dim] = 0;
		err = init_mat(size, dim, out);
	}

	free(size);

	return err;
}


err_ret ones(char **inp, struct vari *var, struct matrix **out) {
	//call zeros and just replace all the inp
	err_ret err = zeros(inp, var, out);

	if(!err) {
		for(uint64_t i = 0; i < out[0]->len; ++i)
			out[0]->elements[i] = 1;
	}

	return err;
}


err_ret rand_mat(char **inp, struct vari *var, struct matrix **out) {
	err_ret err = zeros(inp, var, out);

	if(!err) {
		for(uint64_t i = 0; i < out[0]->len; ++i)
			out[0]->elements[i] = (ele)rand() / RAND_MAX;

	}

	return err;
}


err_ret linspace(char **inp, struct vari *var, struct matrix **out) {
	int argNo = numberOfArgs(inp);
	if(argNo != 3)
		return -2;

	struct vari *tmp = NULL;
	err_ret err = cpy_var(var, &tmp);
	if(err)
		return err;

	err = sya(inp[0], tmp);
	if(err)
		goto err_ret;
	
	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele a = tmp->ans->elements[0];


	err = sya(inp[1], tmp);
	if(err)
		goto err_ret;
	
	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele b = tmp->ans->elements[0];


	err = sya(inp[2], tmp);
	if(err)
		goto err_ret;

	if(tmp->ans->dim != 1) {
		err = -10;
		goto err_ret;
	}
	ele len = tmp->ans->elements[0];


	if( (len < 0) || ((len - floor(len)) > 0) ) {
		err = -4;
		goto err_ret;
	}

	uint16_t size[3] = {len, 1, 0};

	err = init_mat(size, 2, out);
	if(err)
		goto err_ret;

	ele step = (b - a) / (len - 1);

	for(uint64_t i = 0; i < out[0]->len; ++i)
		out[0]->elements[i] = step * (ele) i + a;

 err_ret:
	free_var(tmp);
  
	return err;
}


err_ret extractValue(char **inp, int var_ind, struct vari *var, struct matrix **out) {
	if(!inp[0][0])
		return -5;

	struct vari *tmp = NULL;
	err_ret err = cpy_var(var, &tmp);
	if(err)
		return err;
	
	int dim = numberOfArgs(inp);    

	if(dim == 1) { //if the number of inps is 1
		err = sya(inp[0], tmp);
		if(err)
			goto err_ret;

		err = cpy_mat(tmp->ans, out);
		if(err)
			goto err_ret;

		//out is a matrix that holds indices
		for(uint64_t i = 0; i < out[0]->len; ++i) {

			--(out[0]->elements[i]);
			//check that the inp is within bound
			if((uint64_t)out[0]->elements[i] >= tmp->value[var_ind]->len) {
				err = -11;
				free_mat(*out);
				goto err_ret;
			}
		}

		//if the number of inps is equal to dimension
	} else if(dim == tmp->value[var_ind]->dim) {
		uint16_t *loc = malloc((dim + 1) * sizeof(*loc));
		if(!loc) {
			err = -6;
			goto err_ret;
		}

		for(uint8_t i = 0; i < dim; ++i) {
			err = sya(inp[i], tmp);
			if(err) {
				free(loc);
				goto err_ret;
			}

			//check that the inp is one dimensional
			if(tmp->ans->dim != 1) {
				err = -10;
				free(loc);
				goto err_ret;
			}

			//location is 1 indexed, while sub2ind is 0 indexed
			//so subtract 1 to 0 index
			loc[i] = tmp->ans->elements[0] - 1;

			//check that each sublocation is also within bounds
			if(loc[i] >= tmp->value[var_ind]->size[i]) {
				err = -11;
				free(loc);
				goto err_ret;
			}
		}

		loc[dim] = 0;

		uint64_t ind = sub2ind(loc, tmp->value[var_ind]->size,
				       tmp->value[var_ind]->dim);
		free(loc);

		//check index is within bound
		err = ind < tmp->value[var_ind]->len ?
			init_scalar(ind, out) : -11;
	} else {
		err  = -11;
	}

 err_ret:
	free_var(tmp);

	return err;
}


err_ret chk_var(char *buffer, char *inp, int *iter, struct vari *var, struct stack *num, struct stack *ch, struct matrix **out) {
	err_ret err = 0;

	int len = strlen(buffer);

	int k = 0;

	char **separatedString = NULL;

	if(buffer[len - 1] == '(') {

		buffer[len - 1] = '\0';

		k = find_var(var, buffer);

		separatedString = sep_str(inp, "()[]", ",", iter, &err);
		if(err)
			return err;

		if(k < 0) {
			err = find_user_fun(buffer, separatedString, var, out);
			freeDoubleArray(separatedString);
			return err;
		}


		err = extractValue(separatedString, k, var, out);

		if(!(err < 0)) {
			push(num, var->value[k]);
			push(num, *out);

			push(ch, &O_STRUCT[eReference]);
			*out = NULL;
		}

		freeDoubleArray(separatedString);

	} else {
		k = find_var(var, buffer);
		if(k >= 0) {
			var->value[k]->var = 1;
			push(num, var->value[k]);
		} else if(!var->f_assign) {
			var->f_assign = 1;
			if(k == -1) {
				k = 0;

			} else if(k == -2) {
				k = var->count + 1;

			} else {
				err = -5;
			}
			//if assignment goes wrong, the variable name gets malloc'd
			//but doesn't get assigned to, this ensures that if another
			//assignment happens, the previous failed assignment is free'd
			if(var->name[k] != NULL) {
				free(var->name[k]);
				free(var->value[k]);

				var->name[k] = NULL;
				var->value[k] = NULL;
			}

			var->name[k] = malloc((len + 1) * sizeof(*var->name[k]));
			if(!var->name[k])
				return -6;

			strcpy(var->name[k], buffer);

			var->value[k] = malloc(sizeof(*var->value[k]));
			if(!var->value[k])
				return -6;

			var->value[k]->var = 1;
			var->value[k]->size = NULL;
			var->value[k]->elements = NULL;

			push(num, var->value[k]);
		} else {
			err = -5;
			var->f_assign = 0;
		}

		if(err == -5) {
			//buffer includes the '(', if it's there, replace with 0
			if(buffer[len - 1] == '(') {
				separatedString = sep_str(inp, "()[]", ",", iter, &err);
				if(err)
					return err;
				
				buffer[len - 1] = '\0';
				err = find_user_fun(buffer, separatedString, var, out);
				freeDoubleArray(separatedString);
			}
		}
	}

	return err;
}


/*
 * remove spaces from string, front and back
 */
char *removeSpaces(char *inp) {
	int len = strlen(inp);

	int i = 0;
	for(; inp[i] == ' '; ++i);

	for(int j = len - 1; inp[j] == ' '; --j)
		inp[j] = 0;

	return &inp[i];
}


/*
 * print to stdout, formatting is similar to matlab
 */
err_ret printLine(char **inp, struct vari *var) {
	struct vari *tmp = NULL;
	err_ret err = cpy_var(var, &tmp);
	if(err)
		return err;

	int argNo = numberOfArgs(inp);

	//loop over every argument
	for(int i = 0; (i < argNo) && !err; ++i) {
		int len = strlen(inp[i]);

		//check if the string is quote limited
		int string = 0;

		//iterators for counting from the front/back of string
		int front = 0;
		int back = 0;

		//check if there is a quote in beginning of string, or spaces then a quote
		if(inp[i][0] == '"') {
			string = 1;
		} else {
			for(front = 0; inp[i][front] == ' '; ++front) {
				if(inp[i][front+1] == '"')
					string = 1;
			}
		}

		//check if there is a quote at the end of the string
		//or spaces then a quote (going backwards)
		if(inp[i][len - (back + 1)] == '"') {
			++string;
		} else {
			for(back = 0; inp[i][len - (back + 1)] == ' '; ++back) {
				if(inp[i][len - (back + 1)] == '"')
					++string;
			}
		}


		//if there are quotes at the beginning/end of string, this is true
		if(string) {
			//null terminate it to write over the quote
			inp[i][len - (back + 1)] = '\0';
			if(string == 2) {

				//check if there's a new line
				if((inp[i][len - (back + 3)] == '\\')
				   && (inp[i][len - (back + 2)] == 'n')) {
					//write over the \ in new line
					inp[i][len - (back + 3)] = '\0';

					//print with new line
					printf("%s\n", inp[i] + front + 1);

				} else {
					//print without new line
					printf("%s", inp[i] + front + 1);
				}
			} else {
				//if there's no second quote to match
				err = -9;
			}
		} else { //no quotes, just a variable or expression

			//calculate expression and print, print variables this way
			err = sya(inp[i], tmp);
			if(!err)
				print_mat(tmp->ans);
		}
	}

	free_var(tmp);
	if(!err)
		err = -1;


	//-1 is the error code for no output from sya
	return err;
}


char **sep_str(char *inp, char const *const lim, char const *const delim, int *iter, err_ret *error) {

	//increment inp to the first parenthesis
	inp += (*iter + 1);

	//length of string
	int len = 0;

	//the number of types of delimiters
	int delimiterType = strlen(delim);

	//the number of delimiters in inp
	int delimiterCount = 0;

	//number of types of limiters
	//assume that the number of limiters is going to be even
	//there will always be a left and right end
	int limiterType = strlen(lim) / 2;

	//find where parenthesis are closed
	//also count delimiters
	for(int a = 0; inp[len]; ++len) {
		if(inp[len] == '(')
			++a;

		if(inp[len] == ')')
			--a;

		//break when the parenthesis are balanced
		if(!a)
			break;


		for(int j = 0; j < delimiterType; ++j) {
			if(inp[len] == delim[j]) {
				++delimiterCount;
				break;
			}
		}
	}

	//increment iterator length amount, to the
	//char after the end paren
	*iter += (len + 1);

	//location of end paren, and skipping first paren
	char *inp_end = inp + len;
	++inp;
	
	//assume that each delimiter will have its own string
	//also account for an end NULL pointer
	//this will always be greater than or equal to the
	//actual required amount of memory
	char **sep = malloc(sizeof(*sep) * (delimiterCount + 2));
	__MALLOC_CHECK(sep, *error);

	//count of the number of elements
	int subString = 0;

	if(!delimiterCount) {
		sep[0] = calloc(len, sizeof(**sep));
		__MALLOC_CHECK(sep[0], *error);
		strncpy(*sep, inp, inp_end - inp);
	} else {

		//counter for each of the limiters
		int *limiterCount = calloc(limiterType, sizeof(*limiterCount));
		__MALLOC_CHECK(limiterCount, *error);

		//last index where a delimiter was found
		int cur_len = 0;

		//loop counter
		int k = 0;

		//separate string, by delimiters
		//however, only separate if the limiters
		//like parenthesis or brackets are closed
		for(; inp + k != inp_end; ++k) {
			//count to check that all limiters are balanced
			int allCount = 0;

			//check that each limiter is balanced
			//limiterCount is 0 if the pair is balanced
			for(int l = 0; l < limiterType; ++l) {
				if(inp[k] == lim[l * 2])
					++limiterCount[l];

				if(inp[k] == lim[(l * 2) + 1])
					--limiterCount[l];

				allCount |= limiterCount[l];
			}

			//if the current char is a delimiter
			//and the limiters are balanced, means
			//that the delimiter delimits the arguments
			//of the main function, and not arguments of
			//any sub functions within
			if(strchr(delim, inp[k]) && !allCount) {
				//malloc the number of characters in between each delimiter
				sep[subString] = malloc(sizeof(**sep) * ((k - cur_len) + 1));
				//sep[subString] = calloc((k - cur_len) + 1, sizeof(**sep));
				__MALLOC_CHECK(sep[subString], *error);

				//copy from the previous delimiter to the next one
				strncpy(sep[subString], inp + cur_len, k - cur_len);

				//null terminate string
				sep[subString][k - cur_len] = '\0';

				//set the current delimiter to the one just found
				cur_len = k + 1;

				//increment the number of subStrings
				++subString;
			}
		}

		free(limiterCount);

		sep[subString] = malloc(sizeof(**sep) * ((k - cur_len) + 1));
		__MALLOC_CHECK(sep[subString], *error);

		if(strchr(delim, inp[cur_len]))
			++inp;
		
		strncpy(sep[subString], inp + cur_len, k - cur_len);
		sep[subString][k - cur_len] = '\0';
	}

	++subString;
	sep[subString] = NULL;

	return sep;
}


void freeDoubleArray(char **inp) {
	for(int i = 0; inp[i]; ++i)
		free(inp[i]);

	free(inp);
}
