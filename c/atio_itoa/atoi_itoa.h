#ifndef __ATOI_ITOA__
#define __ATOI_ITOA__

#include <stddef.h>

int Atoi (const char* str, unsigned char base);


/*
value
    Value to be converted to a string.
str
    Array in memory where to store the resulting null-terminated string.
base
    Numerical base used to represent the value as a string, between 2 and 36, where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary. 
    
Returns
A pointer to the resulting null-terminated string, same as parameter str.
*/
char*  Itoa (int value, char* str, unsigned char base );



void PrintIntersectionFirstTwoWithExclusionThird(const char* arr_use_1, size_t size1, const char* arr_use_2, size_t size2, const char* arr_exclude_3, size_t size3);



#endif /* __ATOI_ITOA__ */

