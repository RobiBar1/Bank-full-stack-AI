#ifndef __POINTERS1_H__
#define __POINTERS1_H__
#include <stddef.h> /*size_t type */

void SwapInt(int* x, int* y);
void CopyArray(const int* array, int* dest_array, size_t array_len);
void PrintTypesAddress(void);
void SwapTwoSize_t(size_t* one, size_t* other);
void SwapTwoPtrSize_t(size_t** one, size_t** other);

#endif /*__POINTERS1_H__*/
