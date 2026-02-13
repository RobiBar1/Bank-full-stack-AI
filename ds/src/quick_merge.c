#include <assert.h>      /* assert  */
#include <stdlib.h>      /* malloc  */

#include "quick_merge.h" /* our api */

static int CheckifFound(int* arr, size_t index, int num_to_find)
{
    assert (NULL != arr);

    if (arr[index] == num_to_find)
    {
        return index;
    }

    return -1;
}

int BinarySerch(int* arr, size_t size, int num_to_find)
{
    size_t left = 0;
    size_t right = size - 1;
    size_t mid = 0;

    int index_of_num = 0;

    assert (NULL != arr);

    while (left <= right)
    {   
        mid = left + (right - left) / 2;

        if (0 >= arr[mid])
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
