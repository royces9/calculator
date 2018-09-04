#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"
#include "variables.h"

#include "operator.h"
#include "operatorUtility.h"
#include "sya.h"


//shunting yard algorithm
error_return sya(char *input, vari *var) {

	//iterators
	//main loop iterator
	uint16_t main_iter = 0;

	//character buffer iterator
	uint16_t char_iter = 0;

	//operator buffer iterator
	uint16_t oper_iter = 0;


	//error checking int
	error_return error = 0;


	//to check if the '-' char is subtraction or a negative
	int8_t negativeCheck = 0;

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
	numberStack *num_stack = newNumberStack();

	//stack for operators
	operatorStack *oper_stack = newOperatorStack();


	//main loop
	//iterates through the input string, apply shunting-yard algorithm
	for(; input[main_iter] && !error; ++main_iter) {
		switch(type[main_iter]){

		case 1: //alpha numerics
			//reset bufferOper counter
			oper_iter = 0;
			//put all consecutive alphanumeric characters in a buffer
			bufferLetters[char_iter++] = input[main_iter];

			//for valid numbers/variables/functions
			if((type[main_iter + 1] == 2) || (type[main_iter + 1] == 0)){
				bufferLetters[char_iter] = '\0';

				//if the buffer is all numbers
				if(checkNumbers(bufferLetters)) {
					pushn(initScalar(strtod(bufferLetters, NULL), &error), num_stack);

				} else { //check if command is a function or variable
					if(input[main_iter + 1] == '(')
						bufferLetters[char_iter++] = '(';

					bufferLetters[char_iter] = '\0';
					error = findFunction(bufferLetters, num_stack, oper_stack, var, &negativeCheck, &main_iter, input);
				}

				//reset bufferLetters counter
				char_iter = 0;

			} //end if

			negativeCheck = 1; //negative check for the '-' char, which can be minus or negative
			break;

      
		case 2: //operator characters
			//reset bufferLetters counter
			char_iter = 0;

			//all consecutive operator characters put into a buffer
			bufferOper[oper_iter++] = input[main_iter];

			//checks if the current buffer concatenated with the
			//next character is an operator, if not, go into "if"
			if(checkOperator(bufferOper, input[main_iter + 1]) == OPERATOR_COUNT) {
				bufferOper[oper_iter] = '\0';

				//find the corresponding operator
				error = findOperator(bufferOper, num_stack, oper_stack, var, &negativeCheck);

				//reset bufferOper counter
				oper_iter = 0;
			}
			break;

		case 4: //"[]"
			//reset letters and oper counters
			char_iter = 0;
			oper_iter = 0;
			pushn(extractMatrix(var, &main_iter, input, &error), num_stack);
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
		while(oper_stack->top > -1) {
			if( (error = execNum(num_stack, var, popch(oper_stack))) ) {
				freeOperatorStack(oper_stack);
				freeNumberStack(num_stack);
				var->assignFlag = 0;
				return error;
			}
		}

		free(var->ans->size);
		free(var->ans->elements);

  
		//copy num_stack->stk[0] to var->ans
		//if num_stack->stk is occupied, and
		//if num_stack->stk[0] is not NULL
		if((num_stack->top > -1) && (num_stack->stk[0]->size)) {
			var->ans->length = num_stack->stk[0]->length;
			var->ans->dimension = num_stack->stk[0]->dimension;

			var->ans->elements = malloc(sizeof(*var->ans->elements) * var->ans->length);
			memcpy(var->ans->elements, num_stack->stk[0]->elements, sizeof(*var->ans->elements) * var->ans->length);

			var->ans->size = malloc(sizeof(*var->ans->size) * (var->ans->dimension + 1));
			memcpy(var->ans->size, num_stack->stk[0]->size, sizeof(*var->ans->size) * (var->ans->dimension + 1));

		} else {
			error = -5;

			var->ans->size = NULL;
			var->ans->elements = NULL;
		}
	}

	//free stacks
	freeOperatorStack(oper_stack);
	freeNumberStack(num_stack);

	//reset assignment
	var->assignFlag = 0;
	return error;
}


//print out errors if there are any
void errorReport(error_return error) {
	if(error < -1){ //error codes for -2 or lower
		printf("\nError %d:\n", error);
		switch(error) {

		case -2: printf("Incorrect number of function arguments."); break;
		case -3: printf("Mismatched parenthesis."); break;
		case -4: printf("Invalid expression."); break;
		case -5: printf("Invalid function or variable name."); break;
		case -6: printf("Malloc error."); break;
		case -7: printf("Invalid operator."); break;
		case -8: printf("File does not exist."); break;
		case -9: printf("Mismatched quotation marks."); break;
		case -10:printf("Matrix dimensions do not match."); break;
		case -11:printf("Out of matrix bounds."); break;
		case -12:printf("No output variable."); break;
		case -13:printf("Invalid assignment."); break;
		default: break;
		}
		printf("\n\n");
	}
}


//check if the string is a number/variable
error_return checkNumbers(char *input) {
	for(int i = 0; input[i]; ++i) {
		if(((input[i] < '0') && (input[i] != '.')) ||
		   (input[i] > '9') ||
		   (!input[i]) )
			return 0;
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
