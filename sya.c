#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "operator.h"
#include "operatorUtility.h"
#include "sya.h"


//shunting yard algorithm
err_ret sya(char *input, struct vari *var) {
	//error checking int
	err_ret err = 0;

	//to check if the '-' char is subtraction or a negative
	int negativeCheck = 0;

	//Error checking
	//count for the number of parentheses and brackets
	int parenthesisCount = 0; 
	int bracketCount = 0;

	int len = 0;

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
			return e_mismatched_brackets;

		if(input[len] == '[')
			++bracketCount;
		if(input[len] == ']')
			--bracketCount;

		if(bracketCount < 0)
			return e_mismatched_brackets;
	}

	//if the number of left and right ends are the same
	//the count variables will be 0
	if(parenthesisCount || bracketCount)
		return e_mismatched_brackets;

	//if input string ends in an operator, return error
	if(strchr(".[,+-/*^(=&|~<>",input[len - 1]))
		return e_invalid_expr;


	//buffers for characters and operators
	char *bufferLetters = calloc(len + 1, sizeof(*bufferLetters));
	if(!bufferLetters)
		return e_malloc;

	char *bufferOper = calloc(len + 1, sizeof(*bufferOper));
	if(!bufferOper)
		return e_malloc;


	int8_t *type = malloc(sizeof(*type) * (len + 1));
	if(!type)
		return e_malloc;

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
			if(!type[l + 1])
				type[l] = 2;
			else
				type[l] = type[l + 1];
			++l;
		}
	}

	type[len] = 0;

	//stack for output numbers
	struct stack *num = new_stk(512);
	if(!num)
		return e_malloc;

	//stack for operators
	struct stack *op = new_stk(512);
	if(!op)
		return e_malloc;

	//iterators
	//character buffer iterator
	int char_iter = 0;

	//operator buffer iterator
	int oper_iter = 0;

	//main loop
	//iterates through the input string, apply shunting-yard algorithm
	for(int i = 0; input[i] && !err; ++i) {
		switch(type[i]){

		case 1: //alpha numerics
			//reset bufferOper counter
			oper_iter = 0;
			//put all consecutive alphanumeric characters in a buffer
			bufferLetters[char_iter] = input[i];
			++char_iter;
			
			//for valid numbers/variables/functions
			if( (type[i + 1] == 2) || (!type[i + 1]) ){
				bufferLetters[char_iter] = '\0';

				//if the buffer is all numbers
				if(chk_num(bufferLetters)) {
					double buf_num = strtod(bufferLetters, NULL);
					struct matrix *temp;
					err = init_scalar(buf_num, &temp);
					if(err)
						break;

					push(num, temp);
				} else { //check if command is a function or variable
					if(input[i + 1] == '(') {
						bufferLetters[char_iter] = '(';
						++char_iter;
					}

					bufferLetters[char_iter] = '\0';
					err = find_fun(bufferLetters, num, op, var, &negativeCheck, &i, input);
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
			bufferOper[oper_iter] = input[i];
			++oper_iter;

			//checks if the current buffer concatenated with the
			//next character is an operator, if not, go into "if"
			int op_ind = 0;
			err = chk_op(bufferOper, input[i + 1], &op_ind);
			if(err)
				break;

			if(op_ind == (OPERATOR_COUNT)) {
				bufferOper[oper_iter] = '\0';
				//find the corresponding operator
				err = find_op(bufferOper, num, op, var, &negativeCheck);

				//reset bufferOper counter
				oper_iter = 0;
			}

			break;

		case 4: //"[]"
			//reset letters and oper counters
			char_iter = 0;
			oper_iter = 0;
			struct matrix *a = NULL;
			err = ext_mat(var, &i, input, &a);
			if(!err)
				push(num, a);
			break;
      
		case -1:
			err = e_invalid_expr;
			break;

		}//end of switch
	}//end of for

	free(type);
	free(bufferLetters);
	free(bufferOper);

	if(!err){

		//while the operator and number stack are occupied, keep executing
		while(op->top > -1) {
			err = ex_num(num, var, pop(op));
			if(err)
				goto err_ret;
		}

		free(var->ans->size);
		free(var->ans->elements);

		//copy num_stack->stk[0] to var->ans
		//if num_stack->stk is occupied, and
		//if num_stack->stk[0] is not NULL
		if((num->top > -1) && (((struct matrix **)num->stk)[0]->size)) {
			var->ans->len = ((struct matrix **)num->stk)[0]->len;
			var->ans->dim = ((struct matrix **)num->stk)[0]->dim;

			var->ans->elements = malloc(sizeof(*var->ans->elements) * var->ans->len);
			if(!var->ans->elements)
				return e_malloc;

			memcpy(var->ans->elements,
			       ((struct matrix **)num->stk)[0]->elements,
			       sizeof(*var->ans->elements) * var->ans->len);

			var->ans->size = malloc(sizeof(*var->ans->size) * (var->ans->dim + 1));
			if(!var->ans->size)
				return e_malloc;

			memcpy(var->ans->size,
			       ((struct matrix **)num->stk)[0]->size,
			       sizeof(*var->ans->size) * (var->ans->dim + 1));

		} else {
			err = e_invalid_func;
			var->ans->size = NULL;
			var->ans->elements = NULL;
		}
	}
 err_ret:
	//free stacks
	free_stk(num, (void (*) (void *))&free_mat);
	free_stk(op, NULL);

	//reset assignment
	var->f_assign = 0;
	return err;
}


//check if the string is a number/variable
int chk_num(char *input) {
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
err_ret chk_op(char *a, char b, int *out) {

	int len = strlen(a);
	char *buffer = malloc((len + 2) * sizeof(*buffer));
	if(!buffer)
		return e_malloc;

	strcpy(buffer, a);

	buffer[len] = b;
	buffer[len + 1] = '\0';

	*out = search_str(buffer, OPERATOR_LIST);
	free(buffer);

	return 0;
}


int in_range(char a, char left, char right) {
	return (a >= left) && (a <= right);
}

int is_alphanumeric(char a) {
	if(in_range(a, '0', '9'))
		return 1;

	if(in_range(a, 'a', 'z'))
		return 1;

	if(in_range(a, 'A', 'Z'))
		return 1;

	if(a == '_')
		return 1;

	return 0;
}

//checks the type of character

//alpha numeric is 1
//operators are 2
//'.' is 3, can be either operator or alpha
//"[]" is 4, matrix operator
//misc characters are 0, just ignore em
//nonsupported characters are -1?
int chk_t(char a) {
	switch(a) {
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
		if(is_alphanumeric(a))
			return 1;
		
		return -1;
	}
}
