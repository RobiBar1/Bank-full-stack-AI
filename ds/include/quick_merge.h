#ifndef __QUICK_MERGE_H__
#define __QUICK_MERGE_H__

#include <unistd.h>    /* ssize_t */

/*
* @pre: arr != NULL.
* @pre: the arr is sorted.
* @return: positive num that is the index of number if found in arr.
* @return: negetive num if not found in arr.
*/
ssize_t BinarySearchIter(const int* sorted_arr, size_t num_elements, int num);

/*
* @pre: arr != NULL.
* @pre: the arr is sorted.
* @return: positive num that is the index of number if found in arr.
* @return: negetive num if not found in arr.
*/

ssize_t BinarySearchRecursive(const int* sorted_arr, size_t num_elements, int num);
/*
* @pre: arr != NULL.
* @return: zero if sort success, non zero if memory allocate fail.
*/
int MergeSort(int* arr, size_t size);

/*
* @pre: arr != NULL.
* @return: zero if sort success, non zero if memory allocate fail.
*/
void Qsort(void* arr, size_t arr_len, size_t element_size, int (*compar)(const void*, const void*));

#endif /* __QUICK_MERGE_H__ */
