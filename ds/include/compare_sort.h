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

#endif /* __SORTINGS_H__ */
