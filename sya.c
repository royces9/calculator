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
err_ret sya(char *input, vari *var) {

	//iterators
	//character buffer iterator
	uint16_t char_iter = 0;

	//operator buffer iterator
	uint16_t oper_iter = 0;

	//error checking int
	err_ret error = 0;


	//to check if the '-' char is subtraction or a negative
	int8_t negativeCheck = 0;

	//Error checking
	//count for the number of parentheses and brackets
	int16_t parenthesisCount = 0; 
	int16_t bracketCount = 0;

	int16_t len = 0;

	//count left and right end parens/brackets, check that they match
	//also measure length of string
	for(; input[len]; ++len) {
		//increment for left end
		//decrement for right end
		if(input[len] == '(')
			++parenthesisCount;
		if(input[len] == ')')
			--parenthesisCount;

		if(parenthesisCount < 0)
			return -3;

		if(input[len] == '[')
			++bracketCount;
		if(input[len] == ']')
			--bracketCount;

		if(bracketCount < 0)
			return -3;
	}


	//if the number of left and right ends are the same
	//the count variables will be 0
	if(parenthesisCount || bracketCount)
		return -3;


	//if input string ends in an operator, return error
	if(strchr(".[,+-/*^(=&|~<>",input[len - 1]))
		return -4;


	//buffers for characters and operators
	char *bufferLetters = calloc(len + 1, sizeof(*bufferLetters));
	__MALLOC_CHECK(bufferLetters, error);

	char *bufferOper = calloc(len + 1, sizeof(*bufferOper));
	__MALLOC_CHECK(bufferOper, error);


	int8_t *type = malloc(sizeof(*type) * (len + 1));

	//assign a type to each char in string
	//for the switch in the main loop
	for(int l = 0; input[l]; ++l) {
		type[l] = chk_t(input[l]);

		//3 is the '.' character
		//it's possible to determine if
		//it is a decimal point or
		//part of a scalar operator
		//by checking the next character
		if(type[l] == 3){
			type[l + 1] = chk_t(input[l + 1]);
			type[l] = type[l + 1];
			++l;
		}
	}

	type[len] = 0;

	//stack for output numbers
	numberStack *num_stk = newNumberStack();

	//stack for operators
	operatorStack *op_stk = newOperatorStack();

	//main loop
	//iterates through the input string, apply shunting-yard algorithm
	for(uint16_t i = 0; input[i] && !error; ++i) {
		switch(type[i]){

		case 1: //alpha numerics
			//reset bufferOper counter
			oper_iter = 0;
			//put all consecutive alphanumeric characters in a buffer
			bufferLetters[char_iter++] = input[i];

			//for valid numbers/variables/functions
			if( (type[i + 1] == 2) || (!type[i + 1]) ){
				bufferLetters[char_iter] = '\0';

				//if the buffer is all numbers
				if(chk_num(bufferLetters)) {
					pushn(init_scalar(strtod(bufferLetters, NULL), &error), num_stk);

				} else { //check if command is a function or variable
					if(input[i + 1] == '(')
						bufferLetters[char_iter++] = '(';

					bufferLetters[char_iter] = '\0';
					error = find_fun(bufferLetters, num_stk, op_stk, var, &negativeCheck, &i, input);
				}

				//reset bufferLetters counter
				char_iter = 0;

			} //end if

			//negative check for the '-' char, which can be minus or negative
			negativeCheck = 1;
			break;

      
		case 2: //operator characters
			//reset bufferLetters counter
			char_iter = 0;

			//all consecutive operator characters put into a buffer
			bufferOper[oper_iter++] = input[i];

			//checks if the current buffer concatenated with the
			//next character is an operator, if not, go into "if"
			int check_op = chk_op(bufferOper, input[i + 1], &error);
			if(error)
				break;

			//if(checkOperator(bufferOper, input[i + 1]) == OPERATOR_COUNT) {
			if(check_op == OPERATOR_COUNT) {
				bufferOper[oper_iter] = '\0';

				//find the corresponding operator
				error = find_op(bufferOper, num_stk, op_stk, var, &negativeCheck);

				//reset bufferOper counter
				oper_iter = 0;
			}

			break;

		case 4: //"[]"
			//reset letters and oper counters
			char_iter = 0;
			oper_iter = 0;
			matrix *a = extractMatrix(var, &i, input, &error);

			if(!error)
				pushn(a, num_stk);

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
		while(op_stk->top > -1) {
			if( (error = ex_num(num_stk, var, popch(op_stk))) ) {
				freeOperatorStack(op_stk);
				freeNumberStack(num_stk);
				var->assign = 0;
				return error;
			}
		}

		free(var->ans->size);
		free(var->ans->elements);

  
		//copy num_stack->stk[0] to var->ans
		//if num_stack->stk is occupied, and
		//if num_stack->stk[0] is not NULL
		if((num_stk->top > -1) && (num_stk->stk[0]->size)) {
			var->ans->len = num_stk->stk[0]->len;
			var->ans->dim = num_stk->stk[0]->dim;

			var->ans->elements = malloc(sizeof(*var->ans->elements) * var->ans->len);
			memcpy(var->ans->elements, num_stk->stk[0]->elements, sizeof(*var->ans->elements) * var->ans->len);

			var->ans->size = malloc(sizeof(*var->ans->size) * (var->ans->dim + 1));
			memcpy(var->ans->size, num_stk->stk[0]->size, sizeof(*var->ans->size) * (var->ans->dim + 1));

		} else {
			error = -5;

			var->ans->size = NULL;
			var->ans->elements = NULL;
		}
	}

	//free stacks
	freeOperatorStack(op_stk);
	freeNumberStack(num_stk);

	//reset assignment
	var->assign = 0;
	return error;
}


//print out errors if there are any
void err_rep(err_ret error) {
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
err_ret chk_num(char *input) {
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
int chk_op(char *a, char b, err_ret *error) {

	int len = strlen(a);
	char *buffer = malloc(sizeof(*buffer) * (len + 2));
	__MALLOC_CHECK(buffer, *error);

	strcpy(buffer, a);

	buffer[len] = b;
	buffer[len + 1] = '\0';

	int out = search_op(buffer);
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
int8_t chk_t(char a) {
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
	case '%':
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
