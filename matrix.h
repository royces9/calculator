#ifndef MATRIX
#define MATRIx

typedef struct matrix{
	//number of elements;
	uint64_t len;

	//size of the array
	uint16_t *size;

	//elements of the matrix
	ele *elements;

	//number of dimensions
	uint8_t dim;

	//variable flag
	int8_t var;
} matrix; 


matrix *init_mat(uint16_t *size, uint8_t dim, err_ret *error);
matrix *init_scalar(ele e);
matrix *cpy_mat(matrix *src, err_ret *error);
matrix *cat_mat(matrix *a, matrix *b, uint8_t dim, err_ret *error);

void free_mat(matrix *m);
void print_two_d(const matrix *m, int offset);
void print_mat(const matrix *m);

uint64_t get_len(uint16_t *size, uint8_t dim);
uint64_t sub2ind(uint16_t *location, uint16_t *size, uint8_t dim);

err_ret is_scalar(matrix *m);
err_ret is_vec(matrix *m);

err_ret cmp_size(uint16_t *a, uint16_t *b, uint8_t dimA, uint8_t dimB);
err_ret chk_inner(matrix *a, matrix *b);

#endif //MATRIX
