/*
Writer: Robi
Checker: ?
Date: 14.02.2026
*/

#include <assert.h>      /* assert   */
#include <stdlib.h>      /* malloc   */
#include <stdint.h>      /* SIZE_MAX */ 

#include "quick_merge.h" /* our api  */

#define FINISH_SORTED 0
#define ALLOCATE_FAIL 1

/*============================= Binary Serch Iterative ============================*/
ssize_t BinarySearchIter(const int* arr, size_t size, int num_to_find)
{
    size_t left = 0;
    size_t right = size - 1;
    size_t mid = 0;

    assert (NULL != arr);

    while (left <= right && right != SIZE_MAX)
    {   
        mid = left + (right - left) / 2;

        if (num_to_find == arr[mid])
        {
            return mid;
        }

        if (arr[mid] < num_to_find)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return -1;
}

/*============================= Binary Serch Recursive ============================*/

static int HelperRecursive(const int* arr,int mid , int left, int right, int num_to_find)
{
    assert (NULL != arr);

    if (left > right)
    {
        return -1;
    }

    mid = left + (right - left) / 2;

    if(arr[mid] == num_to_find)
    {
        return mid;
    }

    if (arr[mid] < num_to_find)
    {
        left = mid + 1;
    }
    else
    {
        right = mid - 1;
    }

    return HelperRecursive(arr, mid, left, right, num_to_find);
}

ssize_t BinarySearchRecursive(const int* arr, size_t size, int num_to_find)
{
    assert (NULL != arr);

    return HelperRecursive(arr, 0, 0, size - 1, num_to_find);
}
/*=================================================================================*/


/*================================== Merge Sort ===================================*/
static void CopyElements(int *dest, const int *src, size_t count)
{
    if (0 == count--)
    {
        return;
    }

    *dest++ = *src++;
    CopyElements(dest, src, count);
}

static void Conquer(int* arr, const int* left_half, int left_remain
                 , int* right_half, int right_remain)
{
    assert (NULL != arr);
    assert (NULL != left_half);
    assert (NULL != right_half);

    if (0 == left_remain)
    {
        return;
    }

    if (0 == right_remain)
    {
        CopyElements(arr, left_half, left_remain);
        return;
    }

    if (*left_half <= *right_half)
    {
        *arr++ = *left_half++;
        Conquer(arr, left_half, left_remain - 1, right_half, right_remain);
    }
    else
    {
        *arr++ = *right_half++;
        Conquer(arr, left_half, left_remain, right_half, right_remain - 1);
    }
}


static void DivideConquer(int* arr, int* arr_left, size_t left, size_t right)
{
    size_t mid = 0;
    size_t left_size = 0;

    assert (NULL != arr);
    assert (NULL != arr_left);

    if (left < right)
    {
        mid = left + (right - left) / 2;
        DivideConquer(arr, arr_left, left,  mid);
        DivideConquer(arr, arr_left, mid + 1, right);

        left_size = mid - left + 1;

        CopyElements(arr_left, arr + left, left_size);
        Conquer(arr + left, arr_left, left_size, arr + mid + 1, right - mid);
    }
}

int MergeSort(int* arr, size_t size)
{
    int* left_arr = NULL;
    size_t left_arr_size = 0;

    assert (NULL != arr);

    if (2 > size)
    {
        return FINISH_SORTED;
    }

    left_arr_size = (size + 1) / 2;
    left_arr = (int*)malloc(sizeof(int) * left_arr_size);
    if (NULL == left_arr)
    {
        return ALLOCATE_FAIL;
    }

    DivideConquer(arr, left_arr, 0, size - 1);
    free(left_arr); left_arr = NULL;

    return FINISH_SORTED;
}
/*=================================================================================*/


/*================================== Quick Sort ===================================*/

static void SwapBytes(char *first, char *second, size_t remaining_bytes)
{
    char temp = 0;

    assert (NULL != first);
    assert (NULL != second);

    if (0 == remaining_bytes--)
    {
        return;
    }

    temp = *first;
    *first = *second;
    *second = temp;

    SwapBytes(++first, ++second, remaining_bytes);
}


static size_t ScanPartition(char* base, size_t elem_size,
    int (*cmp_func)(const void*, const void*), size_t pivot_index
    , size_t store_index, size_t scan_index)
{

    assert(NULL != base);
    assert(NULL != cmp_func);
    assert(0 != elem_size);

    if (scan_index >= pivot_index)
    {
        return store_index;
    }

    if (0 >= cmp_func(base + scan_index * elem_size, base + pivot_index * elem_size))
    {
        if (store_index != scan_index)
        {
            SwapBytes(base + store_index * elem_size, base + scan_index * elem_size
                    , elem_size);
        }

        return ScanPartition(base, elem_size, cmp_func, pivot_index, ++store_index
                           , ++scan_index);
    }

    return ScanPartition(base, elem_size, cmp_func, pivot_index, store_index
                       , ++scan_index);
}


static size_t Partition(char* base, size_t elem_size,
    int (*cmp_func)(const void*, const void*),
    size_t low, size_t high)
{
    size_t pivot_final_pos = 0;

    assert(NULL != base);
    assert(NULL != cmp_func);
    assert(0 != elem_size);

    pivot_final_pos = ScanPartition(base, elem_size, cmp_func, high, low, low);
    if (pivot_final_pos != high)
        {
            SwapBytes(base + pivot_final_pos * elem_size,
                 base + high * elem_size, elem_size);
        }

    return pivot_final_pos;
}

static void SortRange(char* base, size_t elem_size,
    int (*cmp_func)(const void*, const void*),
    size_t low, size_t high)
{
    size_t pivot_pos = 0;

    assert(NULL != base);
    assert(NULL != cmp_func);
    assert(0 != elem_size);

    if (low >= high)
    {
        return;
    }

    pivot_pos = Partition(base, elem_size, cmp_func, low, high);

    if (pivot_pos > low)
    {
        SortRange(base, elem_size, cmp_func, low, pivot_pos - 1);
    }
    if (pivot_pos < high)
    {
        SortRange(base, elem_size, cmp_func, pivot_pos + 1, high);
    }
}


void Qsort(void* arr, size_t arr_size, size_t element_size_in_bytes, 
     int (*cmp_func)(const void* left, const void* right))
{
    assert(NULL != arr);
    assert(NULL != cmp_func);
    assert(0 != element_size_in_bytes);

    if (2 > arr_size)
    {
        return;
    }

    SortRange((char *)arr, element_size_in_bytes, cmp_func, 0, arr_size - 1);
}