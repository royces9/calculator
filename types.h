#ifndef TYPES
#define TYPES

#include <stdint.h>

//macro for malloc failure
#define __MALLOC_CHECK(pointer, error)          \
	if((pointer) == NULL) {			\
		(error) = -6;			\
		return 0;                       \
	}

//put simple type defs here
typedef int8_t err_ret;
typedef double ele;

#define BUFF_SIZE 1024
#include "variables.h"

union fp {
	err_ret (*mult)(char **, struct vari *, struct matrix **);
	err_ret (*m_one)(struct matrix const *const, struct matrix **);
	err_ret (*m_two)(struct matrix const *const, struct matrix const *const, struct matrix **);
	ele (*s_one)(ele);
	ele (*s_two)(ele, ele);
	void *p;
};


struct oper { //struct for operators, +, -, etc
	//function pointer
	union fp fp;

	int m_enum;

	//order of operation, higher is higher priority
	uint8_t order;

	//number of arguments it takes, generally two or one
	uint8_t argNo;

	//1 if matrix operator, 0 if scalar
	uint8_t mat_op;
};


#include "matrix.h"
#include "stack.h"

#endif //TYPES
