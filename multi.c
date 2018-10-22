#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"

#include "sya.h"
#include "operatorUtility.h"
#include "multi.h"


uint8_t numberOfArgs(char **inp) {
	uint8_t i = 0;
	for(; inp[i]; ++i); //empty for
	return i;
}


matrix *deri(char **inp, vari *var, err_ret *error) {
	//copy global struct to a local variable struct
	vari *tmp = cpy_var(var, error);

	matrix *ans = NULL;

	//check the number of inps is correct
	if(numberOfArgs(inp) != 4) {
		*error = -2;
		goto err_ret;
	}

	//set both the point and step size
	if((*error = sya(inp[2], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele point = tmp->ans->elements[0];


	if((*error = sya(inp[3], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele h = tmp->ans->elements[0];

  
	//set up a dummy variable specified by user  
	char *tmp_var = removeSpaces(inp[1]);
	matrix *temp = init_scalar(point + h);
	if(!temp) {
		*error = -6;
		goto err_ret;
	}
	
	int var_ind = set_var(tmp, tmp_var, temp, error);

	//f(x+h)
	if((*error = sya(inp[0], tmp)))
		goto err_ret;

	ele out = tmp->ans->elements[0];

	//sets the dummy variable equal to x-h
	tmp->value[var_ind]->elements[0] = point - h;

	//f(x-h)
	if((*error = sya(inp[0], tmp)))
		goto err_ret;

	//f(x+h) - f(x-h)
	out -= tmp->ans->elements[0];
  
	ans = init_scalar(out / (2 * h));
	if(!ans)
		*error = -6;
 err_ret:
	free_var(tmp);

	return ans;
}


matrix *inte(char **inp, vari *var, err_ret *error) {
	//check number of arguments
	if(numberOfArgs(inp) != 5) {
		*error = -2;
		return NULL;
	}

	matrix *out = NULL;

	//copy global struct to a local variable struct
	vari *tmp = cpy_var(var, error);
	int var_ind = 0;

	//get number of steps, and step size
	if((*error = sya(inp[2], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele a = tmp->ans->elements[0];


	if((*error = sya(inp[3], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele b = tmp->ans->elements[0];


	if((*error = sya(inp[4], tmp)))
		goto err_ret;
	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele number = tmp->ans->elements[0];

	//calculate step size
	ele step = (b - a) / number;

	//set dummy variable
	char *tmp_var = removeSpaces(inp[1]);

	matrix *temp = init_scalar(0);
	if(!temp) {
		*error = -6;
		goto err_ret;
	}
		
	var_ind = set_var(tmp, tmp_var, temp, error);
	//calculate integral using composite Simpson's

	number = floor(number/2); //halve the steps

	ele sum = 0;
	for(int i = 1; i <= number; ++i) {
		//f(x_2i-2)
		tmp->value[var_ind]->elements[0] = a + (((2 * i) - 2) * step);
		if((*error = sya(inp[0], tmp)))
			goto err_ret;
		sum += tmp->ans->elements[0];


		//4*f(x_2i-1)
		tmp->value[var_ind]->elements[0] = a + (((2 * i) - 1) * step);
		if((*error = sya(inp[0], tmp)))
			goto err_ret;
		sum += (4 * tmp->ans->elements[0]);


		//f(x_2i)
		tmp->value[var_ind]->elements[0] = a + ((2 * i) * step);
		if((*error = sya(inp[0], tmp)))
			goto err_ret;
		sum += tmp->ans->elements[0];
	}


	out = init_scalar(sum * (step / 3));
	if(!out)
		*error = -6;

 err_ret:
	free_var(tmp);

	//return integral
	return out;
}


matrix *solve(char **inp, vari *var, err_ret *error) {
	//check number of arguments
	if(numberOfArgs(inp) != 4) {
		*error = -2;
		return NULL;
	}

	matrix *ans = NULL;

	//copy global struct to a local variable struct
	vari *tmp = cpy_var(var, error);
  
	double const delta = 0.000001;

	/*
	  inp[0] = function
	  inp[1] = variable
	  inp[2] = initial guess
	  inp[3] = tolerance
	*/

	//set dummy variable

	//set initial guess and the tolerance
	if((*error = sya(inp[2], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}

	char *tmp_var = removeSpaces(inp[1]);
	int var_ind = set_var(tmp, tmp_var, cpy_mat(tmp->ans, error), error);

	if((*error = sya(inp[3], tmp)))
		goto err_ret;
	if(tmp->ans->dim != 1) {
		*error = -10;
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
	ele out = 0;
	ele inter = 0;

	//if the difference between iterations is
	//less than the tolerance, break out of loop
	while(fabs(test) > h) {
		if((*error = sya(inp[0], tmp)))
			goto err_ret;

		out = tmp->ans->elements[0];

		tmp->value[var_ind]->elements[0] -= delta;
		if((*error = sya(inp[0], tmp)))
			goto err_ret;

		inter = tmp->ans->elements[0];

		test = (delta * out) / (out - inter);
		tmp->value[var_ind]->elements[0] -= test;

		//if counter overflows and goes back to 0
		//this is true, max value is 65535 (2 bytes)
		if(!(++counter)) {
			*error = -12;
			goto err_ret;
		}
	}


	ans = cpy_mat(tmp->value[var_ind], error);

 err_ret:
	free_var(tmp);

	return ans;
}


matrix *zeros(char **inp, vari *var, err_ret *error) {
	uint8_t dim = numberOfArgs(inp);
	uint16_t *newSize = NULL;

	//only one inp, make a square matrix of that size
	//or if it's a matrix, make one of that size
	switch(dim) {
	case 1:
 		if((*error = sya(inp[0], var)))
			break;

		//check that the one inp is a scalar
		if(is_scalar(var->ans)) {
			//change dimension to make square matrix
			dim = 2;

			newSize = malloc(sizeof(*newSize) * (dim + 1));
			__MALLOC_CHECK(newSize, *error);

			if(var->ans->elements[0]) {
				newSize[0] = var->ans->elements[0];
				newSize[1] = var->ans->elements[0];
				newSize[2] = 0;
			} else {
				*error = -11;
			}

		} else if(is_vec(var->ans)) {
			dim = var->ans->dim;
			newSize = malloc(sizeof(*newSize) * (dim + 1));
			__MALLOC_CHECK(newSize, *error);
	
			uint8_t i = 0;
			for(; i < var->ans->len; ++i) {
				if( !(var->ans->elements[i]) ) {
					newSize[i] = var->ans->elements[i];
				} else {
					*error = -11;
					break;
				}
			}

			newSize[i] = 0;
		} else {
			*error = -11;
		}
		break;

	default:
		newSize = malloc(sizeof(*newSize) * (dim + 1));
		__MALLOC_CHECK(newSize, *error);

		for(uint8_t i = 0; i < dim; ++i) {
			if((*error = sya(inp[i], var)))
				break;

			if(var->ans->dim != 1 || !var->ans->elements[0]) {
				*error = -10;
				break;
			}

			newSize[i] = var->ans->elements[0];
		}
	}

	matrix *out = NULL;
	if( !(*error)) {
		newSize[dim] = 0;
		out = init_mat(newSize, dim, error);
	}

	free(newSize);

	return out;
}


matrix *ones(char **inp, vari *var, err_ret *error) {
	//call zeros and just replace all the inp
	matrix *out = zeros(inp, var, error);

	if( !(*error) ) {
		for(uint64_t i = 0; i < out->len; ++i)
			out->elements[i] = 1;
	}

	return out;
}


matrix *rand_mat(char **inp, vari *var, err_ret *error) {
	matrix *out = zeros(inp, var, error);
	if( !(*error) ) {
		for(uint64_t i = 0; i < out->len; ++i)
			out->elements[i] = (ele)rand() / RAND_MAX;

	}

	return out;
}


matrix *linspace(char **inp, vari *var, err_ret *error) {
	int argNo = numberOfArgs(inp);
	if(argNo != 3) {
		*error = -2;
		return NULL;
	}

	matrix *out = NULL;

	vari *tmp = cpy_var(var, error);

	if((*error = sya(inp[0], tmp)))
		goto err_ret;
	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele a = tmp->ans->elements[0];

	if((*error = sya(inp[1], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}
	ele b = tmp->ans->elements[0];

	if((*error = sya(inp[2], tmp)))
		goto err_ret;

	if(tmp->ans->dim != 1) {
		*error = -10;
		goto err_ret;
	}

	ele len = tmp->ans->elements[0];


	if( (len < 0) || ((len - floor(len)) > 0) ) {
		*error = -4;

	} else {
		uint16_t size[3] = {len, 1, 0};

		out = init_mat(size, 2, error);
		if(*error)
			goto err_ret;

		ele step = (b - a) / (len - 1);

		for(uint64_t i = 0; i < out->len; ++i)
			out->elements[i] = step * (ele) i + a;
	}

 err_ret:
	free_var(tmp);
  
	return out;
}


matrix *extractValue(char **inp, int var_ind, vari *var, err_ret *error) {
	if(!inp[0][0]) {
		*error = -4;
		return NULL;
	}

	uint8_t dim = numberOfArgs(inp);    
	matrix *out = NULL;

	vari *tmp = cpy_var(var, error);
	if(!tmp)
		goto err_ret;

	if(dim == 1) { //if the number of inps is 1
		if((*error = sya(inp[0], tmp)))
			goto err_ret;

		out = cpy_mat(tmp->ans, error);
		if(*error)
			goto err_ret;

		//out is a matrix that holds indices
		for(uint64_t i = 0; i < out->len; ++i) {

			--(out->elements[i]);
			//check that the inp is within bound
			if((uint64_t)out->elements[i] >= tmp->value[var_ind]->len) {
				*error = -11;
				free_mat(out);
				goto err_ret;
			}
		}

		//if the number of inps is equal to dimension
	} else if(dim == tmp->value[var_ind]->dim) {
		uint16_t *loc = malloc(sizeof(*loc) * (dim + 1));
		__MALLOC_CHECK(loc, *error);

		for(uint8_t i = 0; i < dim; ++i) {
			if((*error = sya(inp[i], tmp))) {
				free(loc);
				goto err_ret;
			}

			//check that the inp is one dimensional
			if(tmp->ans->dim == 1) {
				//location is 1 indexed, while sub2ind is 0 indexed
				//so subtract 1 to 0 index
				loc[i] = tmp->ans->elements[0] - 1;

				//check that each sublocation is also within bounds
				if(loc[i] >= tmp->value[var_ind]->size[i]) {
					*error = -11;
					free(loc);
					goto err_ret;
				}
			} else {
				*error = -10;
				free(loc);
				goto err_ret;
			}
		}
		loc[dim] = 0;

		uint64_t ind = sub2ind(loc, tmp->value[var_ind]->size,
				       tmp->value[var_ind]->dim);
		free(loc);

		//check index is within bound
		if(ind < tmp->value[var_ind]->len) {
			out = init_scalar(ind);
			if(!out)
				*error = -6;
		} else {
			*error = -11;
		}
      
	} else {
		*error  = -11;
	}

 err_ret:
	free_var(tmp);

	return out;
}


err_ret chk_var(const char *buffer, char *inp, uint16_t *iter, vari *var, stack *num, stack *ch) {
	err_ret error = 0;

	uint16_t len = strlen(buffer);

	char *nameBuffer = malloc(sizeof(*nameBuffer) * (len + 1));
	__MALLOC_CHECK(nameBuffer, error);

	//copy buffer so we don't change the original string
	strcpy(nameBuffer, buffer);

	int k = 0;

	matrix *out = NULL;
	char **separatedString = NULL;

	if(nameBuffer[len - 1] == '(') {

		nameBuffer[len - 1] = '\0';

		k = find_var(var, nameBuffer);
		if(k < 0) {
			free(nameBuffer);
			return -5;
		}

		separatedString = sep_str(inp, "()", ",", iter, &error);

		out = extractValue(separatedString, k, var, &error);

		if(!(error < 0)) {
			push(num, var->value[k]);
			push(num, out);

			push(ch, init_op_struct("r", 2, 0, eReference));
		} 

		freeDoubleArray(separatedString);

	} else {
		k = find_var(var, nameBuffer);
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
				error = -5;

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

			var->name[k] = malloc(sizeof(*var->name[k]) * (len + 1));
			__MALLOC_CHECK(var->name[k], error);

			strcpy(var->name[k], nameBuffer);

			var->value[k] = malloc(sizeof(*var->value[k]));
			__MALLOC_CHECK(var->value[k], error);

			var->value[k]->var = 1;
			var->value[k]->size = NULL;
			var->value[k]->elements = NULL;

			push(num, var->value[k]);
		} else {
			error = -5;
			var->f_assign = 0;
		}
	}

	free(nameBuffer);

	return error;
}


/*
 * remove spaces from string, front and back
 */
char *removeSpaces(char *inp) {
	int len = strlen(inp);

	uint16_t i = 0;
	for(; *(inp + i) == ' '; ++i);

	for(uint16_t j = len - 1; inp[j] == ' '; --j) {
		inp[j] = 0;
	}

	return (inp + i);
}


/*
 * print to stdout, formatting is similar to matlab
 */
err_ret printLine(char **inp, vari *var) {
	err_ret error = 0;

	uint8_t argNo = numberOfArgs(inp);

	vari *tmp = cpy_var(var, &error);

	//loop over every argument
	for(uint8_t i = 0; (i < argNo) && !error; ++i) {
		uint16_t len = strlen(inp[i]);

		//check if the string is quote limited
		uint8_t string = 0;

		//iterators for counting from the front/back of string
		uint16_t front = 0;
		uint16_t back = 0;

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
				error = -9;
			}
		} else { //no quotes, just a variable or expression

			//calculate expression and print, print variables this way
			error = sya(inp[i], tmp);
			if(!error)
				print_mat(tmp->ans);
		}
	}

	free_var(tmp);
	if(!error)
		error = -1;


	//-1 is the error code for no output from sya
	return error;
}


char **sep_str(char *inp, char const * const lim, char const * const delim, uint16_t *iter, err_ret *error) {

	//increment inp to the first parenthesis
	inp += (*iter + 1);

	//length of string
	uint16_t len = 0;

	//the number of types of delimiters
	uint8_t delimiterType = strlen(delim);

	//the number of delimiters in inp
	uint16_t delimiterCount = 0;

	//number of types of limiters
	//assume that the number of limiters is going to be even
	//there will always be a left and right end
	uint8_t limiterType = strlen(lim) / 2;

	//find where parenthesis are closed
	//also count delimiters
	for(int16_t a = 0; inp[len]; ++len) {
		if(inp[len] == '(')
			++a;

		if(inp[len] == ')')
			--a;

		//break when the parenthesis are balanced
		if(!a)
			break;


		for(uint8_t j = 0; j < delimiterType; ++j) {
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
	char *inp_end = inp + (strlen(inp) - 1);
	++inp;

	//assume that each delimiter will have its own string
	//also account for an end NULL pointer
	//this will always be greater than or equal to the
	//actual required amount of memory
	char **sep = malloc(sizeof(*sep) * (delimiterCount + 2));
	__MALLOC_CHECK(sep, *error);

	//count of the number of elements
	uint16_t subString = 0;

	if(!delimiterCount) {
		sep[0] = malloc(sizeof(**sep) * (len + 1));
		__MALLOC_CHECK(sep[0], *error);    
		strncpy(*sep, inp, inp_end - inp);
	} else {

		//counter for each of the limiters
		int16_t *limiterCount = calloc(limiterType, sizeof(*limiterCount));
		__MALLOC_CHECK(limiterCount, *error);

		//last index where a delimiter was found
		uint16_t cur_len = 0;

		//loop counter
		uint16_t k = 0;

		//separate string, by delimiters
		//however, only separate if the limiters
		//like parenthesis or brackets are closed
		for(; inp + k != inp_end; ++k) {
			//count to check that all limiters are balanced
			uint8_t allCount = 0;

			//check that each limiter is balanced
			//limiterCount is 0 if the pair is balanced
			for(uint8_t l = 0; l < limiterType; ++l) {
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
				sep[subString] = malloc(sizeof(**sep) * ((k - cur_len)) + 1);
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

	sep[++subString] = NULL;

	return sep;
}

void freeDoubleArray(char **inp) {
	int i = 0;
	for(i = 0; inp[i]; ++i)
		free(inp[i]);

	free(inp);
}
