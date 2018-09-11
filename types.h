#ifndef TYPES
#define TYPES

//macro for malloc failure
#define __MALLOC_CHECK(pointer, error)          \
	if((pointer) == NULL) {			\
		(error) = -6;			\
		return 0;                       \
	}


//put simple type defs here
typedef int8_t error_return;
typedef double element;

#endif //TYPES
