#ifndef MATRIX
#define MATRIX

struct matrix{
	//number of elements;
	long len;

	//elements of the matrix
	ele *elements;

	//size of the array
	int *size;

	//number of dimensions
	uint8_t dim;

	//variable flag
	int8_t var;
};

err_ret init_mat(int *size, uint8_t dim, struct matrix **out);
err_ret init_scalar(ele e, struct matrix **out);
err_ret cpy_mat(struct matrix const *const src, struct matrix **out);
err_ret cat_mat(struct matrix const *const a, struct matrix const *const b, uint8_t dim, struct matrix **out);

void free_mat(struct matrix *m);
void print_two_d(const struct matrix *m, int offset);
void print_mat(const struct matrix *m);

uint64_t get_len(int *size, uint8_t dim);
uint64_t sub2ind(int *location, int *size, uint8_t dim);

err_ret is_scalar(struct matrix const *const m);
err_ret is_vec(struct matrix const *const m);

err_ret cmp_size(int *a, int *b, uint8_t dimA, uint8_t dimB);
err_ret chk_inner(struct matrix *a, struct matrix *b);

#endif //MATRIX
