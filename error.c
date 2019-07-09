#include <stdio.h>

#include "error.h"

char const *const err_str[] = {
			       [e_fatal] = "Fatal error.",
			       [e_func_args] = "Incorrect number of function arguments.",
			       [e_malloc] = "Malloc error.",
			       [e_mismatched_brackets] = "Mismatched parenthesis.",
			       [e_mismatched_quotes] = "Mismatched quotation marks.",
			       [e_file_dne] = "File does not exist.",
			       [e_invalid_func] = "Invalid function or variable name.",
			       [e_invalid_expr] = "Invalid expression.",
			       [e_invalid_oper] = "Invalid operator.",
			       [e_invalid_assign] = "Invalid assignment.",
			       [e_bound] = "Out of matrix bounds.",
			       [e_no_output] = "No output variable.",
			       [e_size_mismatch] = "Matrix dimensions do not match.",
};


//print out errors if there are any
void err_rep(err_ret err) {
	if((err != e_next) && (err != e_exit) && (err!= e_ok)) {
		printf("\nError %d:\n", err);
		puts(err_str[err]);
		puts("");
	}
}
