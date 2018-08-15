#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"
#include "variables.h"

#include "sya.h"
#include "operatorUtility.h"
#include "multi.h"


uint8_t numberOfArgs(char **input) {
	uint8_t i = 0;
	for(; input[i]; ++i); //empty for
	return i;
}


matrix *deri(char **input, vari *var, error_return *error) {
	element out, inter, h, point;
	vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  
	int varIndex = 0;

	//check the number of inputs is correct
	if(numberOfArgs(input) != 4) {
		*error = -2;
		return 0;
	}

	//set both the point and step size
	*error = sya(input[2], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}

	point = varTemp->ans->elements[0];

	*error = sya(input[3], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}

	h = varTemp->ans->elements[0];

  
	//set up a dummy variable specified by user  
	char *dummyVariable = removeSpaces(input[1]);
  
	varIndex = setVariable(varTemp, dummyVariable, initScalar(point + h, error), error);

	//f(x+h)
	*error = sya(input[0], varTemp);
	if(*error) return 0;
	out = varTemp->ans->elements[0];

	//sets the dummy variable equal to x-h
	varTemp->value[varIndex]->elements[0] = point - h;

	//f(x-h)
	*error = sya(input[0], varTemp);
	if(*error) return 0;
	inter = varTemp->ans->elements[0];

	//f(x+h) - f(x-h)
	out -= inter;
  
	freeVari(varTemp);
	return initScalar(out / (2 * h), error);
}


matrix *inte(char **input, vari *var, error_return *error) {

	//check number of arguments
	if(numberOfArgs(input) != 5) {
		*error = -2;
		return 0;
	}

	element step = 0, sum = 0;
	element a, b, number;
	vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
	int varIndex = 0;

	//get number of steps, and step size
	*error = sya(input[2], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}
	a = varTemp->ans->elements[0];

	*error = sya(input[3], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}
	b = varTemp->ans->elements[0];

	*error = sya(input[4], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}
	number = varTemp->ans->elements[0];

	//calculate step size
	step = (b - a) / number;


	//set dummy variable
	char *dummyVariable = removeSpaces(input[1]);

	varIndex = setVariable(varTemp, dummyVariable, initScalar(0, error), error);
	//calculate integral using composite Simpson's

	number = floor(number/2); //halve the steps

	for(int i = 1; i <= number; ++i) {
		//f(x_2i-2)
		varTemp->value[varIndex]->elements[0] = a + (((2 * i) - 2) * step);
		*error = sya(input[0], varTemp);
		if(*error) return 0;
		sum += varTemp->ans->elements[0];


		//4*f(x_2i-1)
		varTemp->value[varIndex]->elements[0] = a + (((2 * i) - 1) * step);
		*error = sya(input[0], varTemp);
		if(*error) return 0;
		sum += (4 * varTemp->ans->elements[0]);


		//f(x_2i)
		varTemp->value[varIndex]->elements[0] = a + ((2 * i) * step);
		*error = sya(input[0], varTemp);
		if(*error) return 0;
		sum += varTemp->ans->elements[0];
	}

	freeVari(varTemp);

	//return integral
	return initScalar(sum * (step / 3), error);
}


matrix *solve(char **input, vari *var, error_return *error) {
	//check number of arguments
	if(numberOfArgs(input) != 4) {
		*error = -2;
		return 0;
	}

	vari *varTemp = copyVari(var, error); //copy global struct to a local variable struct
  
	element out, inter, h;
	double test = 0, delta = 0.000001;
	int varIndex = 0;

	/*
	  input[0] = function
	  input[1] = variable
	  input[2] = initial guess
	  input[3] = tolerance
	*/

	//set dummy variable

	//set initial guess and the tolerance
	*error = sya(input[2], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}


	char *dummyVariable = removeSpaces(input[1]);
	varIndex = setVariable(varTemp, dummyVariable, copyMatrix(varTemp->ans, error), error);

	*error = sya(input[3], varTemp);
	if(*error) return 0;
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return NULL;
	}
	h = varTemp->ans->elements[0];


  
	//ensure test is always greater than h
	//on start
	test = h + 1;

	//counter to keep track of the amount of iterations
	//if it overflows, then break from the loop
	uint16_t counter = 1;

	//solve f(x)=0 for x using Newton's method
	while(fabs(test) > h) { //if the difference between iterations is less than the tolerance, break out of loop
		*error = sya(input[0], varTemp);
		if(*error) return NULL;
		out = varTemp->ans->elements[0];

		varTemp->value[varIndex]->elements[0] -= delta;
		*error = sya(input[0], varTemp);
		if(*error) return NULL;
		inter = varTemp->ans->elements[0];

		test = (delta * out) / (out - inter);
		varTemp->value[varIndex]->elements[0] -= test;

		//if counter overflows and goes back to 0
		//this is true, max value is 65535 (2 bytes)
		if(!(++counter)) {
			*error = -12;
			return NULL;
		}
	}


	matrix *output = copyMatrix(varTemp->value[varIndex], error);
	freeVari(varTemp);
	return output;
}


matrix *zeros(char **input, vari *var, error_return *error) {
	uint8_t dimension = numberOfArgs(input);
	uint16_t *newSize = NULL;

	//only one input, make a square matrix of that size
	//or if it's a matrix, make one of that size
	switch(dimension) {
	case 1:
 		*error = sya(input[0], var);
		if(*error) break;

		//check that the one input is a scalar
		if(var->ans->dimension == 1) {
			//change dimension to make square matrix
			dimension = 2;

			newSize = malloc(sizeof(*newSize) * (dimension + 1));
			if(newSize == NULL) {
				*error = -8;
				break;
			}

			if(var->ans->elements[0]) {
				newSize[0] = var->ans->elements[0];
				newSize[1] = var->ans->elements[0];
				newSize[2] = 0;
			} else {
				*error = -11;
			}

		} else if(isVector(var->ans)) {
			dimension = var->ans->dimension;
			newSize = malloc(sizeof(*newSize) * (dimension + 1));
			if(newSize == NULL) {
				*error = -8;
			}
	
			uint8_t i = 0;
			for(; i < var->ans->length; ++i) {
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
		newSize = malloc(sizeof(*newSize) * (dimension + 1));

		for(uint8_t i = 0; i < dimension; ++i) {
			*error = sya(input[i], var);

			if(var->ans->dimension != 1) {
				*error = -10;
				break;
			}

			newSize[i] = var->ans->elements[0];
		}
	}

	matrix *out = NULL;
	if( !(*error)) {
		newSize[dimension] = 0;
		out = initMatrix(newSize, dimension, error);
	}

	free(newSize);

	return out;
}


matrix *ones(char **input, vari *var, error_return *error) {
	//call zeros and just replace all the input
	matrix *out = zeros(input, var, error);
	if( !(*error) ) {
		for(uint64_t i = 0; i < out->length; ++i) {
			out->elements[i] = 1;
		}
	}

	return out;
}


matrix *randMatrix(char **input, vari *var, error_return *error) {
	matrix *out = zeros(input, var, error);
	if( !(*error) ) {
		for(uint64_t i = 0; i < out->length; ++i) {
			out->elements[i] = (element)rand() / RAND_MAX;
		}

	}

	return out;
}


matrix *linspace(char **input, vari *var, error_return *error) {
	int argNo = numberOfArgs(input);

	matrix *out = NULL;

	if(argNo != 3) {
		*error = -2;
		return out;
	}

	vari *varTemp = copyVari(var, error);

	element a = 0;
	element b = 0;
	element length = 0;

	*error = sya(input[0], varTemp);
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return out;
	}
	a = varTemp->ans->elements[0];

	*error = sya(input[1], varTemp);
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return out;
	}
	b = varTemp->ans->elements[0];


	*error = sya(input[2], varTemp);
	if(varTemp->ans->dimension != 1) {
		*error = -10;
		return out;
	}
	length = varTemp->ans->elements[0];

	uint16_t newSize[3] = {length, 1, 0};

	out = initMatrix(newSize, 2, error);

	element step = (b - a) / (length - 1);

	for(uint64_t i = 0; i < out->length; ++i) {
		out->elements[i] = step * i + a;
	}

	freeVari(varTemp);
  
	return out;
}


matrix *extractValue(char **input, int varIndex, vari *var, error_return *error) {
	matrix *out = NULL;
	uint8_t dimension = numberOfArgs(input);

	if(input[0][0] == 0) {
		*error = -4;
		return NULL;
	}
    
	vari *varTemp = copyVari(var, error);

	if(dimension == 1) { //if the number of inputs is 1
		*error = sya(input[0], varTemp);
		if(*error) {
			freeVari(varTemp);
			return NULL;
		}

		out = copyMatrix(varTemp->ans, error);
		if(*error) {
			freeVari(varTemp);
			return NULL;
		}

		//out is a matrix that holds indices
		for(uint64_t i = 0; i < out->length; ++i) {

			--(out->elements[i]);
			//check that the input is within bound
			if((uint64_t)out->elements[i] >= varTemp->value[varIndex]->length) {
				*error = -11;
				freeVari(varTemp);
				freeMatrix(out);

				return NULL;
			}
		}

		//if the number of inputs is equal to dimension
	} else if(dimension == varTemp->value[varIndex]->dimension) {
		uint16_t *location = malloc(sizeof(*location) * (dimension + 1));

		for(uint8_t i = 0; i < dimension; ++i) {
			*error = sya(input[i], varTemp);
			if(*error) {
				free(location);
				freeVari(varTemp);
				return NULL;
			}

			//check that the input is one dimensional
			if(varTemp->ans->dimension == 1) {
				//location is 1 indexed, while sub2ind is 0 indexed
				//so subtract 1 to 0 index
				location[i] = varTemp->ans->elements[0] - 1;

				//check that each sublocation is also within bounds
				if(location[i] >= varTemp->value[varIndex]->size[i]) {
					*error = -11;
					free(location);
					freeVari(varTemp);
					return NULL;
				}
			} else {
				*error = -10;
				free(location);
				freeVari(varTemp);
				return NULL;
			}
		}
		location[dimension] = 0;

		uint64_t index = sub2ind(location, varTemp->value[varIndex]->size, varTemp->value[varIndex]->dimension);
		free(location);

		//check index is within bound
		if(index < varTemp->value[varIndex]->length) {
			out = initScalar(index, error);
		} else {
			*error = -11;
		}
      
	} else {
		*error  = -11;
	}

	freeVari(varTemp);

	return out;
}


error_return checkVariable(const char *buffer, int8_t *tok, char *input, uint16_t *iterator, vari *var, numberStack *num, operatorStack *ch) {
	error_return error = 0;

	uint16_t varLen = strlen(buffer);

	char *nameBuffer = malloc(sizeof(*nameBuffer) * (varLen + 1));
	__MALLOC_CHECK(nameBuffer, error);

	//copy buffer so we don't change the original string
	strcpy(nameBuffer, buffer);

	int k = 0;

	matrix *out = NULL;
	char **separatedString = NULL;

	if(nameBuffer[varLen - 1] == '(') {

		nameBuffer[varLen - 1] = '\0';

		k = findVariable(var, nameBuffer);
		if(k < 0) {
			free(nameBuffer);
			return -5;
		}

		separatedString = separateString(input, "()", ",", iterator, &error);

		out = extractValue(separatedString, k, var, &error);

		if(!(error < 0)) {
			pushn(var->value[k], num);
			pushn(out, num);

			pushch(initOperatorStruct("r", 2, 0, eReference), ch);
		} 

		freeDoubleArray(separatedString);

	} else {
		k = findVariable(var, nameBuffer);

		if(k >= 0) {
			var->value[k]->variable = 1;
			pushn(var->value[k], num);

		} else if(!var->assignFlag) {
			var->assignFlag = 1;
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

			var->name[k] = malloc(sizeof(*var->name[k]) * (varLen + 1));
			__MALLOC_CHECK(var->name[k], error);

			strcpy(var->name[k], nameBuffer);

			var->value[k] = malloc(sizeof(*var->value[k]));
			__MALLOC_CHECK(var->value[k], error);

			var->value[k]->variable = 1;
			var->value[k]->size = NULL;
			var->value[k]->elements = NULL;

			pushn(var->value[k], num);
		} else {
			error = -5;
			var->assignFlag = 0;
		}
	}

	free(nameBuffer);

	return error;
}


/*
 * remove spaces from string, front and back
 */
char *removeSpaces(char *input) {
	int length = strlen(input);

	uint16_t i = 0;
	for(; *(input + i) == ' '; ++i);

	for(uint16_t j = length - 1; input[j] == ' '; --j) {
		input[j] = 0;
	}

	return (input + i);
}


/*
 * print to stdout, formatting is similar to matlab
 */
error_return printLine(char **input, vari *var) {
	error_return error = 0;

	uint8_t argNo = numberOfArgs(input);

	vari *varTemp = copyVari(var, &error);

	//loop over every argument
	for(uint8_t i = 0; i < argNo; ++i) {
		uint16_t len = strlen(input[i]);

		//check if the string is quote limited
		uint8_t string = 0;

		//iterators for counting from the front/back of string
		uint16_t front = 0;
		uint16_t back = 0;

		//check if there is a quote in beginning of string, or spaces then a quote
		if(input[i][0] == '"') {
			string = 1;
		} else {
			for(front = 0; input[i][front] == ' '; ++front) {
				if(input[i][front+1] == '"')
					string = 1;
			}
		}

		//check if there is a quote at the end of the string, or spaces then a quote (going backwards
		if(input[i][len-(back+1)] == '"') {
			++string;
		} else {
			for(back = 0; input[i][len-(back+1)] == ' '; ++back) {
				if(input[i][len-(back+1)] == '"')
					++string;
			}
		}


		//if there are quotes at the beginning/end of string, this is true
		if(string) {
			input[i][len-(back+1)] = '\0'; //null terminate it to write over the quote
			if(string == 2) {

				//check if there's a new line
				if((input[i][len-(back+3)] == '\\') && (input[i][len-(back+2)] == 'n')) {
					input[i][len-(back+3)] = '\0'; //write over the \ in new line
					printf("%s\n", input[i]+front+1); //print with new line

				} else {
					printf("%s", input[i]+front+1); //print without new line

				}
			} else {
				return -9; //there's no second quote to match, error

			}
		} else { //no quotes, just a variable or expression
			error = sya(input[i], varTemp); //calculate expression and print, print variables this way
			if(error) return error;

			printMatrix(varTemp->ans);

		}
	}

	freeVari(varTemp);

	//-1 is the error code for no output from sya
	return -1;
}


char **separateString(char *input, char const * const limiter, char const * const delimiter, uint16_t *iterator, error_return *error) {

	//increment input to the first parenthesis
	input += (*iterator + 1);

	//length of string
	uint16_t length = 0;

	//the number of types of delimiters
	uint8_t delimiterType = strlen(delimiter);

	//the number of delimiters in input
	uint16_t delimiterCount = 0;

	//number of types of limiters
	//assume that the number of limiters is going to be even
	//there will always be a left and right end
	uint8_t limiterType = strlen(limiter) / 2;

	
	//find where parenthesis are closed
	//also count delimiters
	for(int16_t a = 0; input[length]; ++length) {
		if(input[length] == '(')
			++a;

		if(input[length] == ')')
			--a;

		//break when the parenthesis are balanced
		if(!a)
			break;


		for(uint8_t j = 0; j < delimiterType; ++j) {
			if(input[length] == delimiter[j]) {
				++delimiterCount;
				break;
			}
		}
	}


	//increment iterator length amount, to the
	//char after the end paren
	*iterator += (length + 1);


	//get a copy of the input to mangle
	char *input2 = calloc(length + 1, sizeof(*input2));
	__MALLOC_CHECK(input2, *error);
	strncpy(input2, input + 1, length - 1);


	//assume that each delimiter will have its own string
	//also account for an end NULL pointer
	//this will always be greater than or equal to the
	//actual required amount of memory
	char **separatedString = malloc(sizeof(*separatedString) * (delimiterCount + 2));
	__MALLOC_CHECK(separatedString, *error);

	//count of the number of elements
	uint16_t subString = 0;

	//if there are no delimiters
	if(!delimiterCount) {
		separatedString[0] = malloc(sizeof(**separatedString) * (length + 1));
		__MALLOC_CHECK(separatedString[0], *error);    
		strcpy(*separatedString, input2);

	} else {

		//counter for each of the limiters
		int16_t *limiterCount = calloc(limiterType, sizeof(*limiterCount));
		__MALLOC_CHECK(limiterCount, *error);

		//last index where a delimiter was found
		uint16_t currentLength = 0;

		//loop counter
		uint16_t k = 0;

		//separate string, by delimiters
		//however, only separate if the limiters
		//like parenthesis or brackets are closed
		for(; input2[k]; ++k) {
			//count to check that all limiters are balanced
			uint8_t allCount = 0;

			//check that each limiter is balanced
			//limiterCount is 0 if the pair is balanced
			for(uint8_t l = 0; l < limiterType; ++l) {
				if(input2[k] == limiter[l * 2])
					++limiterCount[l];

				if(input2[k] == limiter[(l * 2) + 1])
					--limiterCount[l];

				allCount |= limiterCount[l];
			}

			//if the current char is a delimiter
			//and the limiters are balanced, means
			//that the delimiter delimits the arguments
			//of the main function, and not arguments of
			//any sub functions within
			if(strchr(delimiter, input2[k]) && !allCount) {
				//malloc the number of characters in between each delimiter
				separatedString[subString] = malloc(sizeof(**separatedString) * ((k - currentLength)) + 1);
				__MALLOC_CHECK(separatedString[subString], *error);

				//copy from the previous delimiter to the next one
				strncpy(separatedString[subString], input2 + currentLength, k - currentLength);

				//null terminate string
				separatedString[subString][k - currentLength] = '\0';

				//set the current delimiter to the one just found
				currentLength = k;

				//increment the number of subStrings
				++subString;
			}

		}

		free(limiterCount);

		separatedString[subString] = malloc(sizeof(**separatedString) * ((k - currentLength) + 1));
		__MALLOC_CHECK(separatedString[subString], *error);
		uint8_t offset = 0;
		if(strchr(delimiter, input2[currentLength]))
			offset = 1;
		
		strncpy(separatedString[subString], input2 + currentLength + offset, k - currentLength);
		separatedString[subString][k - currentLength] = '\0';
	}

	separatedString[++subString] = NULL;
	free(input2);

	return separatedString;
}

void freeDoubleArray(char **input) {
	int i = 0;
	for(i = 0; input[i]; ++i) {
		free(input[i]);
	}
	free(input[i]);
	free(input);
}
