#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "file.h"
#include "userFunctions.h"
#include "functions.h"
#include "multi.h"
#include "sya.h"
#include "operatorUtility.h"
#include "operator.h"


int search_str(char *buffer, char const *const *const list) {
	int i = 0;
	for(; list[i]; ++i)
		if(!strcmp(list[i], buffer))
			break;

	return i;
}


//executes either one argument function or two argument function
err_ret ex_num(struct stack *num, struct vari *var, struct oper *ch) {
	struct matrix *a = NULL;
	struct matrix *b = NULL;
	struct matrix *out = NULL;
	err_ret err = 0;

	switch(ch->argNo) {
	case 1:
		a = pop(num);
		if(a->size) {
			err = mat_one(a, ch, &out);
		} else {
			err = e_invalid_func;
		}

		free_mat(a);
		break;

	case 2:
		b = pop(num);
		a = pop(num);

		if(!a) {
			err = e_invalid_expr;
		} else if(a->size && b->size) {
			err = mat_two(a, b, ch, &out);
		} else {
			err = e_invalid_func;
		}

		free_mat(a);
		free_mat(b);

		break;

	case 3:
		b = pop(num);
		a = pop(num);
		if(a) {
			err = assign(a, b, var, &out);
		} else {
			err = e_invalid_expr;
		}

		free_mat(b);

		break;

	default:
		break;
	}

	if(out && !err) {
		push(num, out);
	} else {
		free_mat(out);
	}

	return err;
}


err_ret mat_one(struct matrix *a, struct oper *ch, struct matrix **out) {
	err_ret err = 0;

	if(!ch->mat_op) {
		int *newSize = malloc(sizeof(*newSize) * (a->dim + 1));
		if(!newSize)
			return e_malloc;

		memcpy(newSize, a->size, sizeof(*newSize) * (a->dim + 1));

		err = init_mat(newSize, a->dim, out);
		free(newSize);

		if(err)
			return err;

		for(long i = 0; i < out[0]->len; ++i)
			out[0]->elements[i] = ch->fp.s_one(a->elements[i]);

	} else {
		err = ch->fp.m_one(a, out);
	}

	return err;
}


err_ret mat_two(struct matrix *a, struct matrix *b, struct oper *ch, struct matrix **out) {
	err_ret err = 0;

	//check if inputs are scalar
	uint8_t aScalar = is_scalar(a);
	uint8_t bScalar = is_scalar(b);

	int check = ch->mat_op;

	if(((aScalar + bScalar) > 0) && check) {
		//if the fp is for a matrix operation
		//change to the scalar operator
		if(ch->m_enum == eMultiplyMatrix) {
			check = 0;
			ch = &O_STRUCT[eMultiply];
		} else if(ch->m_enum == eDivideMatrix) {
			check = 0;
			ch = &O_STRUCT[eDivide];
		}
	}

	if(!check) {
		switch(aScalar + bScalar) {
		case 0: //neither is a scalar

			//check if a and b are the same size
			if(!cmp_size(a->size, b->size, a->dim, b->dim)) {
				err = e_size_mismatch;
				break;
			}

			err = init_mat(a->size, a->dim, out);
			if(err)
				break;

			for(uint64_t i = 0; i < a->len; ++i)
				out[0]->elements[i] =
					ch->fp.s_two(a->elements[i],
						     b->elements[i]);

			break;
      
		case 1:; //only a or b is a scalar
			ele *a_p = a->elements;
			ele *b_p = b->elements;
			ele **inc = NULL;

			if(aScalar) {
				err = cpy_mat(b, out);
				inc = &b_p;
			} else {
				err = cpy_mat(a, out);
				inc = &a_p;
			}

			if(err)
				break;

			for(uint64_t i = 0; i < out[0]->len; ++i, ++(*inc))
				out[0]->elements[i] = ch->fp.s_two(*a_p, *b_p);

			break;

		case 2:; //a and b are both scalars
			ele tmp = ch->fp.s_two(a->elements[0], b->elements[0]);
			err = init_scalar(tmp, out);
			break;

		default:
			err = e_func_args;
			break;
		}

	} else {
		err = ch->fp.m_two(a, b, out);
	}

	return err;
}


err_ret find_fun(char *buffer, struct stack *num, struct stack *ch, struct vari *var, int *tok, int *iter, char *input) {
	char **separatedString = NULL;
	struct matrix *out = NULL;

	int i = search_str(buffer, FUNCTION_LIST);
	err_ret err = 0;

	switch(i) {
	case eQuit:
		return e_exit;

	case eClear:
		for(int i = 0; i <= var->count; ++i){
			var->value[i]->var = 0;
			free_mat(var->value[i]);
			var->value[i] = NULL;

			free(var->name[i]);
			var->name[i] = NULL;
		}
		var->count =  -1;
		puts("\nAll variables cleared\n");
		return e_exit;

	case eList:
		if(var->count > -1) {
			puts("\nVariable List:");
			for(int j = 0; j <= var->count; ++j) {
				printf("%s =", var->name[j]);
				print_mat(var->value[j]);
			}
		} else {
			puts("\nNo variables set\n");
		}

		return e_next;

	case eHelp:
		help_print();
		return e_next;

	case ePi:
		err = init_scalar(M_PI, &out);

		*tok = 1;
		break;

	case eE:
		err = init_scalar(M_E, &out);
		
		*tok = 1;
		break;

	case eAns:
		//copy ans so it doesn't get freed
		err = cpy_mat(var->ans, &out);

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
	case eDet:
		push(ch, &O_STRUCT[eLeftParen]);
		push(ch, &F_STRUCT[i]);
		*tok = 0;
		break;

	case eDeri:
		separatedString = sep_str(input, "()", ",", iter, &err);
		if(err)
			break;
		err = deri(separatedString, var, &out);
		*tok = 0;
		break;

	case eInte:
		separatedString = sep_str(input, "()", ",", iter, &err);
		if(err)
			break;
		err = inte(separatedString, var, &out);
		*tok = 0;
		break;

	case eSolve:
		separatedString = sep_str(input, "()", "," , iter, &err);
		if(err)
			break;
		err = solve(separatedString, var, &out);
		*tok = 0;
		break;

	case eRand:
		separatedString = sep_str(input, "()", ",", iter, &err);
		if(err)
			break;
		err = rand_mat(separatedString, var, &out);
		*tok = 0;
		break;

	case eLinspace:
		separatedString = sep_str(input, "()", ",", iter, &err);
		if(err)
			break;
		err = linspace(separatedString, var, &out);
		*tok = 0;
		break;

	case eZeros:
		separatedString = sep_str(input, "()[]", ",", iter, &err);
		if(err)
			break;
		err = zeros(separatedString, var, &out); 
		*tok = 0;
		break;
    
	case eOnes:
		separatedString = sep_str(input, "()[]", ",", iter, &err);
		if(err)
			break;
		err = ones(separatedString, var, &out);
		*tok = 0;
		break;

		
	case eRun:
		separatedString = sep_str(input, "()", "\0", iter, &err);
		if(err)
			break;
		err = runFile(separatedString, var, 0);
		if(!err) {
			//copy ans matrix so it doesn't get freed
			err = cpy_mat(var->ans, &out);
			*tok = 0;
		}
		break;

	case ePrint:
		separatedString = sep_str(input, "()[]", ",", iter, &err);
		if(err)
			break;
		err = printLine(separatedString, var);
		break;

	case FUNCTION_COUNT: //variables
		//if the variable does not exist
		err = chk_var(buffer, input, iter, var, num, ch, &out);

		break;

	default:
		err = e_invalid_func;
		break;

	}//end of switch

	//if the separated string is not NULL, free it
	if(separatedString)
		freeDoubleArray(separatedString);

	if(!err && out)
		push(num, out);

	return err;
}


err_ret find_op(char *buffer, struct stack *num, struct stack *oper, struct vari *var, int *tok) {
	int i = search_str(buffer, OPERATOR_LIST);
	err_ret err = 0;

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
			       (((struct oper *) top_stk(oper))->order <= 6)
			       && !err ) {
				err = ex_num(num, var, pop(oper));
			}

			push(oper, &O_STRUCT[eAdd]);
		}

		*tok = 0;
		push(oper, &O_STRUCT[eMultiply]);

		struct matrix *temp = NULL;
		err = init_scalar(-1, &temp);
		if(!err)
			push(num, temp);

		break;

	case eExponentMatrix:
		*tok = 0;
		push(oper, &O_STRUCT[eExponentMatrix]);
		break;
    
	case eExponent:
		*tok = 0;
		push(oper, &O_STRUCT[eExponent]);
		break;

	case eLeftParen:
		*tok = 0;
		if(oper->top > -1) {
			if(((struct oper *) top_stk(oper))->order != 15)
				push(oper, &O_STRUCT[eLeftParen]);
		} else {
			push(oper, &O_STRUCT[eLeftParen]);
		}
		break;

	case eRightParen:
		while( (oper->top > -1) && (((struct oper *) top_stk(oper))->m_enum != eLeftParen )) {
			struct oper *top = pop(oper);
			err = ex_num(num, var, top);
		}

		if((oper->top > -1) && (((struct oper *) top_stk(oper))->m_enum == eLeftParen))
			pop(oper);
		
		*tok = 1;
		break;


	case eAssign:
		*tok = 0;
		if((oper->top > -1) &&
		   (((struct oper *) top_stk(oper))->m_enum == eReference) ) {
			var->assign = pop(num);
			pop(oper);
		}

		push(oper, &O_STRUCT[eAssign]);
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
		while((oper->top > -1) &&
		      (((struct oper *) top_stk(oper))->order <= O_STRUCT[i].order) ) {
			struct oper *top = pop(oper);
			err = ex_num(num, var, top);
		}

		*tok = 0;
		push(oper, &O_STRUCT[i]);
		break;

	default:
		return e_invalid_oper;
	}

	return err;
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
				++subMatrices;
				currentLength = j;
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

	++subMatrices;
	out[subMatrices] = NULL;
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

		default:
			break;
		}
	}

	return out;
}


//iter is the counter for the main loop in sya
err_ret ext_mat(struct vari *var, int *iter, char *input, struct matrix **out) {
	err_ret err = 0;

	//input is incremented to start at input[*iter], which is where
	//the first [ should be
	input += (*iter);

	//find where the matrix declaration ends
	//count brackets until they match
	//also get the length of string
	int16_t bracketCount = 1;
	int16_t length = 0;

	for(length = 1; bracketCount && input[length]; ++length) {
		if(input[length] == '[')
			++bracketCount;

		if(input[length] == ']')
			--bracketCount;
	}

	//check that the bracket count is correct
	if(bracketCount)
		return e_mismatched_brackets;

	//increment the main loop counter up to the ']' 
	*iter += (length - 1);

	//the string that will contain every character
	//that contains elements of the matrix
	char *mat_string = malloc(length * sizeof(*mat_string));

	//copy from the first character after the first '['
	strncpy(mat_string, input + 1, length * sizeof(*mat_string));

	//replace the end ']' with a '\0'
	mat_string[length-2] = 0;

	if((mat_string[length-2] == ';') || (mat_string[length-2] == ',')) {
		free(mat_string);
		return e_mismatched_brackets;
	}

	//number stack for creating the matrix
	struct stack *num = new_stk(128);
	if(!num)
		return e_malloc;

	//char array that holds each element of
	//the array and a delimiter (, or ;)
	//at the beginning
	char **sepd_mat = sep_mat(mat_string, countDelimiter(mat_string), &err);

	//free matrixString, not needed anymore
	free(mat_string);

	struct vari *tempVari = NULL;
	err = cpy_var(var, &tempVari);
	if(err)
		goto err_ret;

	err = sya(sepd_mat[0], tempVari);

	struct matrix *temp = NULL;
	err = cpy_mat(tempVari->ans, &temp);
	if(err)
		goto err_ret;

	push(num, temp);

	struct matrix *a = NULL;
	struct matrix *b = NULL;

	for(int i = 1; sepd_mat[i]; ++i) {
		temp = NULL;

		if(!sepd_mat[i][1]) {
			err = e_invalid_expr;
			break;
		}

		switch(sepd_mat[i][0]) {
		case ',':

			err = sya(sepd_mat[i] + 1, tempVari);
			if(err)
				goto err_ret;

			a = pop(num);
			err = cat_mat(a, tempVari->ans, 1, &temp);
			free_mat(a);
			if(err)
				goto err_ret;


			break;

		case ';':
			err = sya(sepd_mat[i] + 1, tempVari);
			if(err)
				goto err_ret;

			err = cpy_mat(tempVari->ans, &temp);
			if(err)
				goto err_ret;

			break;
      
		default:
			err = e_size_mismatch;
			break;
		}

		if(!temp)
			break;

		push(num, temp);
	}

	if( !(err) ) {
		while(num->top > 0) {
			b = pop(num);
			a = pop(num);

			err = cat_mat(a, b, 0, &temp);

			free_mat(a);
			free_mat(b);

			if(!temp)
				break;

			push(num, temp);
		}
		*out = pop(num);

	}

 err_ret:
	free_var(tempVari);
	freeDoubleArray(sepd_mat);
	free_stk(num, (void (*) (void *)) &free_mat);

	return err;
}


void help_print(void) {
	puts("quit - quit program");
	puts("list - list variables");
	puts("clear - clear variables");
	puts("");

	puts("derivative(f(x), x, c, delta)");
	puts("   f(x) - function");
	puts("   x - variable used in function");
	puts("   c - point of the tangent line");
	puts("   delta - the difference used (finite difference)");
	puts("");

	puts("integral(f(x), x, a, b, n)");
	puts("   f(x) - function");
	puts("   x - variable used in function");
	puts("   a - starting point");
	puts("   b - ending point");
	puts("   n - number of partitions (composite Simpson's rule, odd n is equivalent to n-1)");   
	puts("");
	
	puts("solve(f(x), x, guess, delta)");
	puts("   f(x) - function");
	puts("   x - variable used in function");
	puts("   guess - initial guess (Newton's Method)");
	puts("   delta - largest difference allowed between x_n+1 and x_n");
	puts("");
	
	puts("run(file)");
	puts("file - path to a text file");
	puts("   This function parses each line of the file as if it were entered into the console directly, with the exception of \"while\", \"if/else\" and \"end\".");
	puts("   \"while\" - loops until the statement inside the \"while\"'s conditional is false. The inside is executed as if it were entered into the console directly. There may be floating point round off errors.");
	puts("   \"if/else\" - Executes the block of lines inside the \"if\"'s conditional if the statement is true, Otherwise it will execute the \"else\" block.");
	puts("   '#' at the beginning of a line comments out a line");
	puts("   ';' at the end of a line suppresses output");
	puts("");
}


ele add(ele a, ele b) {
	return a + b;
}


ele subtract(ele a, ele b) {
	return a - b;
}


ele multiply(ele a, ele b) {
	return a * b;
}


ele divide(ele a, ele b) {
	return a / b;
}


ele exponent(ele a, ele b) {
	return pow(a, b);
}


ele less(ele a, ele b) {
	return a < b;
}


ele greater(ele a, ele b) {
	return a > b;
}


ele less_equal(ele a, ele b) {
	return a <= b;
}


ele greater_equal(ele a, ele b) {
	return a >=b;
}


ele not_equal(ele a, ele b) {
	return a != b;
}


ele equal(ele a, ele b) {
	return a == b;
}


ele and(ele a, ele b) {
	return a && b;
}


ele or(ele a, ele b) {
	return a || b;
}


ele modulo(ele a, ele b) {
	return (int64_t) a % (int64_t) b;
}
