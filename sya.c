#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operator.h"
#include "stack.h"
#include "sya.h"


//shunting yard algorithm
error_return sya(char *input, vari *var) {

	//iterators
	//main loop iterator
	int i = 0;

	//character buffer iterator
	int j = 0;

	//operator buffer iterator
	int k = 0;


	//error checking int
	error_return error = 0;


	//to check if the '-' char is subtraction or a negative
	int negativeCheck = 0;

	//Error checking
	//count for the number of parentheses and brackets
	int16_t parenthesisCount = 0; 
	int16_t bracketCount = 0;

	int16_t length = 0;

	//count left and right end parens/brackets, check that they match
	//also measure length of string
	for(; input[length]; ++length) {
		//increment for left end
		//decrement for right end
		if(input[length] == '(')
			++parenthesisCount;
		if(input[length] == ')')
			--parenthesisCount;

		if(parenthesisCount < 0)
			return -3;

		if(input[length] == '[')
			++bracketCount;
		if(input[length] == ']')
			--bracketCount;

		if(bracketCount < 0)
			return -3;
	}


	//if the number of left and right ends are the same
	//the count variables will be 0
	if(parenthesisCount || bracketCount)
		return -3;


	//if input string ends in an operator, return error
	if(strchr(".[,+-/*^(=&|~<>",input[length-1]))
		return -4;

	//buffers for characters and operators
	char *bufferLetters = calloc(length + 1, sizeof(*bufferLetters));
	__MALLOC_CHECK(bufferLetters, error);

	char *bufferOper = calloc(length + 1, sizeof(*bufferOper));
	__MALLOC_CHECK(bufferOper, error);


	int8_t *type = malloc(sizeof(*type) * (length + 1));

	//assign a type to each char in string
	//for the switch in the main loop
	for(int l = 0; input[l]; ++l) {
		type[l] = checkType(input[l]);

		//3 is the '.' character
		//it's possible to determine if
		//it is a decimal point or
		//part of a scalar operator
		//by checking the next character
		if(type[l] == 3){
			type[l + 1] = checkType(input[l + 1]);
			type[l] = type[l + 1];
			++l;
		}
	}

	type[length] = 0;

	//stack for output numbers
	numberStack *out = newNumberStack();

	//stack for operators
	operatorStack *operatorStack = newOperatorStack();


	//main loop
	//iterates through the input string, apply shunting-yard algorithm
	for(i = 0; input[i] && !error; ++i) {
		switch(type[i]){

		case 1: //alpha numerics
			//reset bufferOper counter
			k = 0;
			bufferLetters[j++] = input[i]; //put all consecutive alphanumeric characters in a buffer

			//for valid numbers/variables/functions
			if((type[i+1] == 2) || (type[i+1] == 0)){
				bufferLetters[j] = '\0';

				//if the buffer is all numbers
				if(checkNumbers(bufferLetters)) {
					pushn(initScalar(strtod(bufferLetters, NULL), &error), out);

				} else { //check if command is a function or variable
					if(input[i+1] == '(')
						bufferLetters[j++] = '(';

					bufferLetters[j] = '\0';
					error = findFunction(bufferLetters, out, operatorStack, var, &negativeCheck, &i, input);
				}

				//rest bufferLetters counter
				j = 0;

			} //end if

			negativeCheck = 1; //negative check for the '-' char, which can be minus or negative
			break;

      
		case 2: //operator characters
			//reset bufferLetters counter
			j = 0;
			bufferOper[k++] = input[i]; //all consecutive operator characters put into a buffer

			//checks if the current buffer concatenated with the
			//next character is an operator
			if(checkOperator(bufferOper, input[i+1]) == OPERATOR_COUNT) {
				bufferOper[k] = '\0';

				//find the corresponding operator
				error = findOperator(bufferOper, out, operatorStack, var, &negativeCheck);

				//reset bufferOper counter
				k = 0;
			}
			break;

		case 4: //"[]"
			//reset letters and oper counters
			j = 0;
			k = 0;
			pushn(extractMatrix(var, &i, input, &error), out);
			break;

      
		case -1:
			error = -4;
			break;

		}//end of switch
	}//end of for

	free(type);
	free(bufferLetters);
	free(bufferOper);

	if(!error){

		//while the operator and number stack are occupied, keep executing
		while((out->top > -1) && (operatorStack->top > -1)) {
			if( (error = execNum(out, var, popch(operatorStack))) ) {
				freeOperatorStack(operatorStack);
				freeNumberStack(out);
				return error;
			}
		}

		if(var->ans->size != NULL){
			free(var->ans->size);
			free(var->ans->elements);

			var->ans->size = NULL;
			var->ans->elements = NULL;
		}
  
		//copy out->stk[0] to var->ans
		//if out->stk is occupied, and
		//if out->stk[0] is not NULL
		if((out->top > -1) && (out->stk[0]->size)){
			var->ans->length = out->stk[0]->length;
			var->ans->dimension = out->stk[0]->dimension;

			var->ans->elements = malloc(sizeof(*var->ans->elements) * var->ans->length);
			memcpy(var->ans->elements, out->stk[0]->elements, sizeof(*var->ans->elements) * var->ans->length);

			var->ans->size = malloc(sizeof(*var->ans->size) * (var->ans->dimension + 1));
			memcpy(var->ans->size, out->stk[0]->size, sizeof(*var->ans->size) * (var->ans->dimension + 1));

		} else{
			error = -5;

		}
	}

	//free stacks
	freeOperatorStack(operatorStack);
	freeNumberStack(out);

	return error;
}


//print out errors if there are any
void errorReport(error_return error) {
	if(error < -1){ //error codes for -2 or lower
		printf("\nError:\n");
		switch(error) {

		case -2: printf("Incorrect number of function arguments"); break;
		case -3: printf("Mismatched parenthesis"); break;
		case -4: printf("Invalid expression"); break;
		case -5: printf("Invalid function or variable name"); break;
		case -6: printf("Malloc error"); break;
		case -7: printf("Invalid operator"); break;
		case -8: printf("File does not exist"); break;
		case -9: printf("Mismatched quotation marks"); break;
		case -10:printf("Matrix dimensions do not match."); break;
		case -11:printf("Out of matrix bounds."); break;
		case -12:printf("No output variable."); break;
		default: break;
		}
		printf("\n\n");
	}
}


//check if the string is a number/variable
error_return checkNumbers(char *input) {
	for(int i = 0; input[i]; ++i) {
		if(((input[i] < '0') && (input[i] != '.')) || (input[i] > '9') || (!input[i]) ) {
			return 0;
		}
	}
	return 1;
}


//check if "a" concatenated with "b"
//is in the operator array
int checkOperator(char *a, char b) {
	int length = strlen(a);
	char *buffer = malloc(sizeof(*buffer) * (length + 4));
	strcpy(buffer, a);

	buffer[length] = b;
	buffer[length + 1] = '\0';

	int out = searchOperatorArray(buffer);
	free(buffer);

	return out;
}


//checks the type of character

//alpha numeric is 1
//operators are 2
//'.' is 3, can be either operator or alpha
//"[]" is 4, matrix operator
//misc characters are 0, just ignore em
//nonsupported characters are -1?
int8_t checkType(char a) {
	switch(a) {

	case '0' ... '9':
	case 'a' ... 'z':
	case 'A' ... 'Z':
	case '_':
		return 1;

	case '^':
	case '(':
	case '*':
	case '/':
	case '-':
	case '+':
	case ')':
	case '=':
	case '>':
	case '<':
	case '!':
	case '&':
	case '|':
	case '~':
		return 2;

	case '.':
		return 3;

	case '[':
	case ']':
		return 4;
    
	case '\t':
	case '\n':
	case ';':
	case ' ':
		return 0;

	default:
		return -1;
	}
}
