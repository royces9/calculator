#ifndef MATRIX
#define MATRIx

struct matrix{
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
};

struct matrix *init_mat(uint16_t *size, uint8_t dim, err_ret *error);
struct matrix *init_scalar(ele e);
struct matrix *cpy_mat(struct matrix *src);
struct matrix *cat_mat(struct matrix *a, struct matrix *b, uint8_t dim, err_ret *error);

void free_mat(struct matrix *m);
void print_two_d(const struct matrix *m, int offset);
void print_mat(const struct matrix *m);

uint64_t get_len(uint16_t *size, uint8_t dim);
uint64_t sub2ind(uint16_t *location, uint16_t *size, uint8_t dim);

err_ret is_scalar(struct matrix *m);
err_ret is_vec(struct matrix *m);

err_ret cmp_size(uint16_t *a, uint16_t *b, uint8_t dimA, uint8_t dimB);
err_ret chk_inner(struct matrix *a, struct matrix *b);

#endif //MATRIX
