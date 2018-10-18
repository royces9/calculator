#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"
#include "variables.h"

#include "file.h"
#include "userFunctions.h"
#include "functions.h"
#include "multi.h"
#include "sya.h"
#include "operatorUtility.h"
#include "operator.h"

//search in FUNCTION_LIST
int search_fun(char *buffer) {
	for(int i = 0; FUNCTION_LIST[i]; ++i) {
		if(!strcmp(FUNCTION_LIST[i], buffer))
			return i;
	}

	return FUNCTION_COUNT;
}


//search in OPERATOR_COUNT
int search_op(char *buffer) {
	for(int i = 0; OPERATOR_LIST[i]; ++i) {
		if(!strcmp(OPERATOR_LIST[i], buffer))
			return i;
	}

	return OPERATOR_COUNT;
}


//executes either one argument function or two argument function
err_ret ex_num(stack *num, vari *var, op_struct *ch) {
	matrix *a = NULL;
	matrix *b = NULL;
	matrix *out = NULL;
	err_ret error = 0;

	switch(ch->argNo) {
	case 1:
		a = pop(num);
		if(a->size) {
			out = mat_one(a, ch, &error);
		} else {
			error = -5;
		}

		free_mat(a);
		break;

	case 2:
		b = pop(num);
		a = pop(num);
		if(!a) {
			error = -4;
		} else if(a->size && b->size) {
			out = mat_two(a, b, ch, &error);
		} else {
			error = -5;
		}

		free_mat(a);
		free_mat(b);

		break;

	case 3:
		b = pop(num);
		a = pop(num);
		if(!a) {
			error = -4;
		} else {
			out = assign(a, b, var, &error);
		}

		free_mat(b);

		break;

	default:
		break;
	}

	if(out)
		push(num, out);

	free(ch);
	return error;
}


matrix *mat_one(matrix *a, op_struct *ch, err_ret *error) {
	matrix *out;
	err_ret check = 0;

	//check if ch._enum is a scalar operator
	one_arg(0, ch->_enum, &check);

	//ch._enum is in oneArg if check is 0
	if(!check) {
		uint16_t *newSize = malloc(sizeof(*newSize) * (a->dim + 1));
		memcpy(newSize, a->size, sizeof(*newSize) * (a->dim + 1));

		out = init_mat(newSize, a->dim, error);
		free(newSize);

		if(*error)
			return NULL;

		for(uint64_t i = 0; i < out->len; ++i)
			out->elements[i] = one_arg(a->elements[i], ch->_enum, error);
	} else {
		switch(ch->_enum){
		case eEye: out = eye(a, error); break;
		case eSize: out = get_size(a, error); break;
		case eTranspose: out = t_mat(a, error); break;
		case eMax: out = max(a, error); break;
		case eMin: out = min(a, error); break;
		case eAvg: out = avg(a, error); break;
		case eSum: out = sum(a, error); break;
		case eNumel: out = numel(a, error); break;
		case eMagnitude: out = magnitude(a, error); break;
		default: out = cpy_mat(a, error); break;
		}
	}

	return out;
}


matrix *mat_two(matrix *a, matrix *b, op_struct *ch, err_ret *error) {
	matrix *out = NULL;
	err_ret check = 0;

	//check if ch._enum is a scalar operator

	two_arg(1, 1, ch->_enum, &check);

	//check if inputs are scalar
	uint8_t aScalar = is_scalar(a);
	uint8_t bScalar = is_scalar(b);

	if(((aScalar + bScalar) > 0) && check){

		//if the enum is for a matrix operation
		//change to the scalar operator
		switch(ch->_enum){
		case eMultiplyMatrix:
			check = 0;
			ch->_enum = eMultiply;
			break;

		case eDivideMatrix:
			check = 0;
			ch->_enum = eDivide;
			break;
		}
	}

	//ch._enum is in twoArg if check is 0
	if(!check) {
		switch(aScalar + bScalar) {
		case 0: //neither is a scalar

			//check if a and b are the same size
			if(!cmp_size(a->size, b->size, a->dim, b->dim)) {
				*error = -10;
				break;
			}

			out = init_mat(a->size, a->dim, error);
			if(*error)
				break;

			for(uint64_t i = 0; i < a->len; ++i)
				out->elements[i] = two_arg(a->elements[i], b->elements[i], ch->_enum, error);
      
		case 1: //only a or b is a scalar

			if(aScalar) {
				out = cpy_mat(b, error);
				for(uint64_t i = 0; i < out->len; ++i)
					out->elements[i] = two_arg(a->elements[0], b->elements[i], ch->_enum, error);

			} else {
				out = cpy_mat(a, error);
				for(uint64_t i = 0; i < out->len; ++i)
					out->elements[i] = two_arg(a->elements[i], b->elements[0], ch->_enum, error);
			}
			break;

		case 2: //a and b are both scalars

			out = init_scalar(two_arg(a->elements[0], b->elements[0], ch->_enum, error));
			if(!out)
				*error = -6;
			break;

		default:
			*error = -2;
			break;

		}

	} else{
		switch(ch->_enum){
		case eMultiplyMatrix: out = mult_mat(a, b, error); break;
		case eExponentMatrix: out = exp_mat(a, b, error); break;
			//case eDivideMatrix: out = div_mat(a, b, error); break;
		case eReference: out = reference(a, b, error); break;
		default: *error = -10; break;
		}
	}

	return out;
}


err_ret find_fun(char *buffer, stack *num, stack *ch, vari *var, int8_t *tok, uint16_t *iter, char *input) {
	char **separatedString = NULL;
	matrix *out = NULL;

	int i = search_fun(buffer);
	err_ret error = 0;

	switch(i) {
	case eQuit:
		return 1;

	case eClear:
		for(int i = 0; i <= var->count; ++i){
			var->value[i]->var = 0;
			free_mat(var->value[i]);
			var->value[i] = NULL;

			free(var->name[i]);
			var->name[i] = NULL;
		}
		var->count =  -1;
		printf("\nAll variables cleared\n\n");
		return -1;

	case eList:
		if(var->count > -1) {
			printf("\nVariable List:\n");
			for(int j = 0; j <= var->count; ++j) {
				printf("%s =", var->name[j]);
				print_mat(var->value[j]);
			}
		} else {
			printf("\nNo variables set\n\n");
		}
		return -1;

	case eHelp:
		help_print();
		return -1;

	case ePi:
		out = init_scalar(M_PI);
		if(!out)
			error = -6;

		*tok = 1;
		break;

	case eE:
		out = init_scalar(M_E);
		if(!out)
			error = -6;
		
		*tok = 1;
		break;

	case eAns:
		//copy ans so it doesn't get freed
		out = cpy_mat(var->ans, &error);
		*tok = 1;
		break;

	case eSin:
	case eCos:
	case eTan:
	case eLn:
	case eLog:
	case eSqrt:
	case eAsin:
	case eAcos:
	case eAtan:
	case eFloor:
	case eCeil:
	case eRound:
	case eFactorial:
	case eMin:
	case eMax:
	case eAvg:
	case eSum:
	case eEye:
	case eSize:
	case eTranspose:
	case eMagnitude:
	case eNumel:
		push(ch, init_op_struct(FUNCTION_LIST[i], 1, 15, i));
		*tok = 0;
		break;

	case eDeri:
		separatedString = sep_str(input, "()", ",", iter, &error);
		out = deri(separatedString, var, &error);
		*tok = 0;
		break;

	case eInte:
		separatedString = sep_str(input, "()", ",", iter, &error);
		out = inte(separatedString, var, &error);
		*tok = 0;
		break;

	case eSolve:
		separatedString = sep_str(input, "()", "," , iter, &error);
		out = solve(separatedString, var, &error);
		*tok = 0;
		break;

	case eZeros:
		separatedString = sep_str(input, "()[]", ",", iter, &error);
		out = zeros(separatedString, var, &error);
		*tok = 0;
		break;
    
	case eOnes:
		separatedString = sep_str(input, "()[]", ",", iter, &error);
		out = ones(separatedString, var, &error);
		*tok = 0;
		break;

	case eRand:
		separatedString = sep_str(input, "()", ",", iter, &error);
		out = rand_mat(separatedString, var, &error);
		*tok = 0;
		break;

	case eLinspace:
		separatedString = sep_str(input, "()", ",", iter, &error);
		out = linspace(separatedString, var, &error);
		*tok = 0;
		break;

	case eRun:
		separatedString = sep_str(input, "()", "\0", iter, &error);
		error = runFile(separatedString, var, 0);
		if(!error) {
			//copy ans matrix so it doesn't get freed
			out = cpy_mat(var->ans, &error);
			*tok = 0;
		}
		break;

	case ePrint:
		separatedString = sep_str(input, "()[]", ",", iter, &error);
		error = printLine(separatedString, var);
		break;

	case FUNCTION_COUNT: //variables
		//if the variable does not exist
		error = chk_var(buffer, input, iter, var, num, ch);
		if(error == -5){
			int bufferLen = strlen(buffer);
			//buffer includes the '(', if it's there, replaced with 0
			if(buffer[bufferLen - 1] == '(') {
				separatedString = sep_str(input, "()[]", ",", iter, &error);
				buffer[bufferLen - 1] = '\0';
				out = find_user_fun(buffer, separatedString, var, &error);

			} else {
				error = -5;
			}
		}
		break;

	default:
		error = -5;
		break;

	}//end of switch

	//if the separated string is not NULL, free it
	if(separatedString)
		freeDoubleArray(separatedString);

	if(out)
		push(num, out);

	return error;
}


err_ret find_op(char *buffer, stack *num, stack *oper, vari *var, int8_t *tok) {
	int i = search_op(buffer);
	err_ret error = 0;
	/*
	 * Precedence values for operators: Reference wiki page of C/C++ operators
	 * 1
	 * 2 f(x)-calls
	 * 3
	 * 4 ^ ! %
	 * 5
	 * 6 + -
	 * 7
	 * 8 < <= > >=
	 * 9 == !=
	 * 10
	 * 11 &&
	 * 12 ||
	 * 13
	 * 14
	 * 15 parens
	 * 16 =
	 */

	switch(i) {

	case eSubtract:
		if(*tok == 1) {
			//if the stack is occupied, should short-circuit
			while( (oper->top > -1) &&
			       (((op_struct **)oper->stk)[oper->top]->order <= 6)
			       && !error ) {
				error = ex_num(num, var, pop(oper));
			}
			push(oper, init_op_struct("+", 2, 6, eAdd));
		}

		*tok = 0;
		push(oper, init_op_struct("*", 2, 5, eMultiply));
		matrix *temp = init_scalar(-1);
		if(!temp) {
			error = -6;
			break;
		}
			
		push(num, temp);

		break;

	case eExponentMatrix:
		*tok = 0;
		push(oper, init_op_struct("^", 2, 4, eExponentMatrix));
		break;
    
	case eExponent:
		*tok = 0;
		push(oper, init_op_struct(".^", 2, 4, eExponent));
		break;

	case eLeftParen:
		*tok = 0;

		if( (oper->top > -1) && (((op_struct **)oper->stk)[oper->top]->order == 2) ) {
			op_struct *temp = pop(oper);
			push(oper, init_op_struct("(", 0, 15, eLeftParen));
			push(oper, temp);
		} else {
			push(oper, init_op_struct("(", 0, 15, eLeftParen));
		}
		


		break;

	case eRightParen:
		do {
			error = ex_num(num, var, pop(oper));
		} while( (oper->top > -1) && (((op_struct **)oper->stk)[oper->top]->_enum != eLeftParen) );

		*tok = 1;
		free(pop(oper));
		break;


	case eAssign:
		*tok = 0;
		if((oper->top > -1) && (((op_struct **)oper->stk)[oper->top]->_enum == eReference)){
			var->assign = pop(num);

			free(pop(oper));
		}
		push(oper, init_op_struct("=", 3, 16, eAssign));
		break;

	case eAdd:
	case eMultiply:
	case eDivide:
	case eLess:
	case eGreater:
	case eLessEqual:
	case eGreaterEqual:
	case eNotEqual:
	case eEqual:
	case eAnd:
	case eOr:
	case eMultiplyMatrix:
	case eDivideMatrix:
	case eModulo:

		while((oper->top > -1) && (((op_struct **)oper->stk)[oper->top]->order <= operatorPrecedence[i]))
			error = ex_num(num, var, pop(oper));

		*tok = 0;
		push(oper, init_op_struct(buffer, 2, operatorPrecedence[i], i));
		break;

	default:
		return -7;
	}

	return error;
}


//separate a matrix, accounting for sub matrices as input in a matrix
//[[1, 2], 3] or something like that
char **sep_mat(char *input, uint16_t delimiter, err_ret *error) {
	char **out = malloc(sizeof(*out) * (delimiter + 2));
	__MALLOC_CHECK(out, *error);

	//counter for "()[]"
	int bracketCount[2] = {0, 0};

	//the last index where a ',' or ';' was found
	int currentLength = 0;

	//count of the number of subMatrices
	int subMatrices = 0;

	int j = 0;
	for(; input[j]; ++j){
		switch(input[j]){
		case '(':
			++bracketCount[0];
			break;
		case ')':
			--bracketCount[0];
			break;
		case '[':
			++bracketCount[1];
			break;
		case ']':
			--bracketCount[1];
			break;
		case ',':
		case ';':
			if( !( bracketCount[0] || bracketCount[1] ) ){
				out[subMatrices] = malloc(sizeof(**out) * ((j - currentLength) + 2));
				__MALLOC_CHECK(out[subMatrices], *error);
				strncpy(out[subMatrices], input + currentLength, j - currentLength);

				out[subMatrices][j - currentLength] = '\0';
				currentLength = j;
				++subMatrices;
			}
			break;

		default: break;
		}
	}

	//j is the length after the for loop runs
	out[subMatrices] = malloc(sizeof(**out) * ((j - currentLength) + 1));
	__MALLOC_CHECK(out[subMatrices], *error);
	strncpy(out[subMatrices], input + currentLength, j - currentLength);
	out[subMatrices][j - currentLength] = '\0';

	out[++subMatrices] = NULL;
	return out;
}


uint16_t countDelimiter(char *input){
	uint16_t out = 0;
	int16_t bracketCount[2] = {0, 0};

	for(uint16_t i = 0; input[i]; ++i) {
		switch(input[i]){
		case '(':
			++bracketCount[0];
			break;

		case ')':
			--bracketCount[0];
			break;

		case '[':
			++bracketCount[1];
			break;

		case ']':
			--bracketCount[1];
			break;

		case ',':
		case ';':
			if(!(bracketCount[0] || bracketCount[1]))
				++out;

			break;

		default: break;
		}
	}

	return out;
}


//iter is the counter for the main loop in sya
matrix *ext_mat(vari *var, uint16_t *iter, char *input, err_ret *error) {
	//input is incremented to start at input[*iter], which is where
	//the first [ should be
	input += (*iter);

	//find where the matrix declaration ends
	//count brackets until they match
	//also get the length of string
	int16_t bracketCount = 0;
	int16_t length = 0;

	for(length = 0; input[length]; ++length) {
		if(input[length] == '[')
			++bracketCount;

		if(input[length] == ']')
			--bracketCount;

		if(!bracketCount)
			break;
	}

	//check that the bracket count is correct
	if(bracketCount) {
		*error = -4;
		return NULL;
	}

	//increment the main loop counter up to the ']' 
	*iter += (length);

	//the string that will contain every character
	//that contains elements of the matrix
	char *mat_string = malloc(sizeof(*mat_string) * (length));

	//copy from the first character after the first '['
	strncpy(mat_string, input + 1, sizeof(*mat_string) * (length));

	//replace the end ']' with a '\0'
	mat_string[length-1] = 0;

	if((mat_string[length-2] == ';') || (mat_string[length-2] == ',')) {
		free(mat_string);
		*error =  -4;
		return NULL;
	}

	//number stack for creating the matrix
	stack *num = new_stk(128);
	__MALLOC_CHECK(num, *error);

	//char array that holds each element of
	//the array and a delimiter (, or ;)
	//at the beginning
	char **sepd_mat = sep_mat(mat_string, countDelimiter(mat_string), error);

	//free matrixString, not needed anymore
	free(mat_string);

	vari *tempVari = cpy_var(var, error);
	*error = sya(sepd_mat[0], tempVari);

	matrix *temp = cpy_mat(tempVari->ans, error);
	if( *error) {
		free_mat(temp);
		goto err_ret;
	}
		
	push(num, temp);

	matrix *a = NULL;
	matrix *b = NULL;
	matrix *out = NULL;

	for(int i = 1; sepd_mat[i]; ++i) {
		temp = NULL;

		if(!sepd_mat[i][1]) {
			*error = -4;
			break;
		}

		switch(sepd_mat[i][0]) {
		case ',':

			if( !(*error = sya(sepd_mat[i] + 1, tempVari)) ) {
				a = pop(num);
				temp = cat_mat(a, tempVari->ans, 1, error);
				free_mat(a);

				if(*error)
					goto err_ret;
			}

			break;

		case ';':
			if( !(*error = sya(sepd_mat[i] + 1, tempVari)) )
				temp = cpy_mat(tempVari->ans, error);

			break;
      
		default:
			*error = -10;
			break;
		}

		if(temp) {
			push(num, temp);
		} else {
			break;
		}
		
	}

	if( !(*error) ) {
		while(num->top > 0) {
			b = pop(num);
			a = pop(num);

			temp = cat_mat(a, b, 0, error);

			free_mat(a);
			free_mat(b);

			if(temp) {
				push(num, temp);
			} else {
				break;
			}
		}
		out = pop(num);

	}

 err_ret:
	free_var(tempVari);
	freeDoubleArray(sepd_mat);
	free_stk(num, (void (*) (void *)) &free_mat);

	return out;
}


void help_print(void) {
	printf("quit - quit program\n");
	printf("list - list variables\n");
	printf("clear - clear variables\n\n");

	printf("derivative(f(x), x, c, delta)\n   f(x) - function\n   x - variable used in function\n   c - point of the tangent line\n   delta - the difference used (finite difference)\n\n");

	printf("integral(f(x), x, a, b, n)\n   f(x) - function\n   x - variable used in function\n   a - starting point\nb - ending point\n   n - number of partitions (composite Simpson's rule, odd n is equivalent to n-1)\n\n");

	printf("solve(f(x), x, guess, delta)\n   f(x) - function\n   x - variable used in function\n   guess - initial guess (Newton's Method)\n   delta - largest difference allowed between x_n+1 and x_n\n\n");

	printf("run(file)\n   file - path to a text file\n   This function parses each line of the file as if it were entered into the console directly, with the exception of \"while\", \"if/else\" and \"end\".\n    \"while\" - loops until the statement inside the \"while\"'s conditional is false. The inside is executed as if it were entered into the console directly. There may be floating point round off errors.\n   \"if/else\" - Executes the block of lines inside the \"if\"'s conditional if the statement is true, Otherwise it will execute the \"else\" block.\n   '#' at the beginning of\
 a line comments out a line\n   ';' at the end of a line suppresses output\n\n");
}
