#ifndef __SORTINGS_H__
#define __SORTINGS_H__

#include <stddef.h>     /* size_t */

void BubbleSort(int* arr, size_t size);

void SelectionSort(int* arr, size_t size);

void InsertionSort(int* arr, size_t size);

/**
 * @returns 0 on success and non 0 on failure
 **/
int CountingSort(int* arr, size_t size);

/**
 * 
 * @returns 0 on success and non 0 on failure
 **/
int RadixSort(int* arr, size_t size);

/*
* @pre: arr != NULL.
* @return: zero if sort success, non zero if memory allocate fail.
*/
int MergeSortRecursive(int* arr, size_t size);

void QuickSortRecursive(int* arr, size_t arr_size, size_t element_size_in_bytes, 
     int (*cmp_func)(const void* left, const void* right));

#endif /* __SORTINGS_H__ */
