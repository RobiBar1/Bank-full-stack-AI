#include <assert.h>      /* assert   */
#include <stdlib.h>      /* malloc   */
#include <stdint.h>      /* SIZE_MAX */

#include "quick_merge.h" /* our api  */

int BinarySerchIterative(int* arr, size_t size, int num_to_find)
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

static int HelperRecursive(int* arr,int mid , int left, int right, int num_to_find)
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

int BinarySerchRecursive(int* arr, size_t size, int num_to_find)
{
    assert (NULL != arr);

    return HelperRecursive(arr, 0, 0, size - 1, num_to_find);
}

