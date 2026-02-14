#ifndef __QUICK_MERGE_H__
#define __QUICK_MERGE_H__

/*
* @pre: arr != NULL.
* @pre: the arr is sorted.
* @return: positive num that is the index of number if found in arr.
* @return: negetive num if not found in arr.
*/
int BinarySerchIterative(int* arr, size_t size, int num_to_find);

/*
* @pre: arr != NULL.
* @pre: the arr is sorted.
* @return: positive num that is the index of number if found in arr.
* @return: negetive num if not found in arr.
*/
int BinarySerchRecursive(int* arr, size_t size, int num_to_find);

/*
* @pre: arr != NULL.
* @return: zero if sort success, non zero if memory allocate fail.
*/
int MergeSortRecursive(int* arr, size_t size);

/*
* @pre: arr != NULL.
* @return: zero if sort success, non zero if memory allocate fail.
*/
void QuickSortRecursive(int* arr, size_t arr_size, size_t element_size_in_bytes, 
     int (*cmp_func)(const void* left, const void* right));

#endif /* __QUICK_MERGE_H__ */
