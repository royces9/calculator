#ifndef MULTI
#define MULTI

/*
 * count number of strings in the double array
 */
int numberOfArgs(char **input);

/*
 * numerically calculate derivative
 *
 * Arguments:
 * char **input
 *   input[0]: function, kept as a string passed to sya
 *   input[1]: independent variable, in the scope of just this function
 *   input[2]: point, point at which to calculate derivative
 *   input[3]: delta, difference used for finite difference
 *
 * struct vari *var
 *   variable struct for variables in input variables
 *
 * err_ret *error
 *   non zero value if something goes wrong
 */
err_ret deri(char **input, struct vari *var, struct matrix **out);

/*
 * numerically calculate derivative
 * 
 * Arguments:
 * char **input
 *   input[0]: function, kept as a string passed to sya
 *   input[1]: independent variable, string, in the scope of just this function
 *   input[2]: left bound, left integral bound
 *   input[3]: right bound
 *   input[4]: partition count, number of partitions between left and right bounds
 */
err_ret inte(char **input, struct vari *var, struct matrix **out);

/*
 * numerically solve f(x) = 0
 *
 * Arguments:
 * char **input
 *   input[0]: function, kept as a string passed to sya
 *   input[1]: independent variable, string, return values of this that satisfy condition
 *   input[2]: initial guess, inital value for independent variable to start from
 *   input[3]: tolerance, difference between iterations stop
 */
err_ret solve(char **input, struct vari *var, struct matrix **out);

/*
 * make a matrix of zeros/ones/random vals between 0 and 1
 *
 * Arguments:
 * char **input
 *   is the size of the output matrix
 *   can be a single or multiple string(s)
 */
err_ret zeros(char **input, struct vari *var, struct matrix **out);
err_ret ones(char **input, struct vari *var, struct matrix **out);
err_ret rand_mat(char **input, struct vari *var, struct matrix **out);

/*
 * make a column vector with linearlly spaced values
 *
 * Arguments:
 * char **input
 *   input[0]: left limit, first index will have this value
 *   input[1]: right limit, last index will have this value
 *   input[2]: number of elements
 */
err_ret linspace(char **input, struct vari *var, struct matrix **out);

/*
 * get value of variable
 * 
 * Arguments
 * char **input
 *   strings that give a value to an index in a variable
 *
 * int varIndex
 *   the index in vari the variable is in
 */
err_ret extractValue(char **input, int varIndex, struct vari *var, struct matrix **out);

/*
 * check if variable exists
 *
 * Arguments:
 * const char *buffer
 *   string holding variable name
 *
 * int *tok
 *   int to determine if '-' is negative or minus
 */
err_ret chk_var(const char *buffer, char *input, uint16_t *iter, struct vari *var, struct stack *num, struct stack *ch);

/*
 * remove spaces from input
 */
char *removeSpaces(char *input);

/*
 * print output to stdout
 *
 * Arguments:
 * char **input
 *   formatting is similar to matlab print
 */
err_ret printLine(char **input, struct vari *var);

/*
 * separate string
 *
 * Arguments
 * char *input
 *   string to separate
 *
 * char const * const limiter
 *   things like parenthesis and brackets, any delimiters found will be
 *    ignored if the limiters are open
 *
 * char const * const delimiter
 *   the separation points of the string
 *
 * int *iterator
 *   the main loop counter in sya, so it can be incremented the appropriate amount
 */
char **sep_str(char *input, char const * const limiter, char const * const delimiter, uint16_t *iter, err_ret *error);

/*
 * free string output from separateString
 */
void freeDoubleArray(char **input);

#endif //MULTI
