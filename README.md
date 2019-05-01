# Intal - A Big Integer Library in C

A C library for performing arithmetic operations on integers of arbitrary length.

Code Repository: <a target="_blank" href="https://github.com/sumanthvrao/big-integer-library" rel="noreferrer noopener">https://github.com/sumanthvrao/big-integer-library</a>

Internally, the data structure used to achieve this is a struct - a combination of a string and an integer (which keeps track of length). Karatsubha fast multiplication algorithm and Russian peasant multiplication algorithms were experimented with, before coming out with the final multiplication function.

### The following function signatures are supported by the library.

// Create an intal from a string.\
void* intal_create(const char* str)

// Used for printing the intal
char* intal2str(void* intal);

// Does a realloc if it needs a little bigger size. So it's going to allocate a new memory block.\
void* intal_increment(void* intal);

// Decrements the intal and doesn't create a new object\
void* intal_decrement(void* intal);

// All these functions creates a new intal. Client needs to destroy it later.\
void* intal_add(void* intal1, void* intal2);\
void* intal_diff(void* intal1, void* intal2);\
void* intal_multiply(void* intal1, void* intal2);\
void* intal_divide(void* intal1, void* intal2);\
void* intal_pow(void* intal1, void* intal2);\
