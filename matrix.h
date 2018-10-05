#ifndef MATRIX
#define MATRIx

typedef struct matrix{
	//number of elements;
	uint64_t length;

	//size of the array
	uint16_t *size;

	//elements of the matrix
	ele *elements;

	//number of dimensions
	uint8_t dimension;

	//variable flag
	int8_t variable;
} matrix; 


matrix *init_mat(uint16_t *size, uint8_t dimension, err_ret *error);
matrix *init_scalar(ele e, err_ret *error);
matrix *cpy_mat(matrix *src, err_ret *error);
matrix *cat_mat(matrix *a, matrix *b, uint8_t dimension, err_ret *error);

void free_mat(matrix *m);
void print_two_d(const matrix *m, int offset);
void print_mat(const matrix *m);

uint64_t getLength(uint16_t *size, uint8_t dimension);
uint64_t sub2ind(uint16_t *location, uint16_t *size, uint8_t dimension);

err_ret is_scalar(matrix *m);
err_ret is_vec(matrix *m);

err_ret cmp_size(uint16_t *a, uint16_t *b, uint8_t dimA, uint8_t dimB);
err_ret chk_inner(matrix *a, matrix *b);

#endif //MATRIX
