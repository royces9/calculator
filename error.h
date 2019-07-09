#ifndef CALC_ERR
#define CALC_ERR

enum err_enum {
	       e_next = -2,
	       e_exit = -1,
	       e_ok = 0,
	       e_fatal,
	       e_func_args,
	       e_mismatched_brackets,
	       e_mismatched_quotes,
	       e_malloc,
	       e_invalid_func,
	       e_invalid_oper,
	       e_invalid_expr,
	       e_invalid_assign,
	       e_bound,
	       e_file_dne,
	       e_no_output,
	       e_size_mismatch,
};

typedef int err_ret;

void err_rep(err_ret error);

#endif //CALC_ERR
