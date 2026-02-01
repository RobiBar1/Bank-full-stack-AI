#include <assert.h>
#include <stddef.h>

#include "compare_sort.h"

static void swap(int* a, int* b)
{
	int temp = 0;
	
	assert (NULL != a);
	assert (NULL != b);
	
	temp = *a;
	*a = *b;
	*b = temp;
}

void BubbleSort(int *arr, size_t size)
{
    size_t i = 0;
    size_t j = 0;
    int swapped = 0;

    assert (NULL != arr);

    if (2 > size)
    {
        return;
    }

    for (i = 0; i < size - 1; ++i)
    {
        swapped = 0;
        for (j = 0; j < size - 1 - i; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        
        if (0 == swapped)
        {
            return;
        }
    }
}

void SelectionSort(int *arr, size_t size)
{
    size_t i = 0;
    size_t j = 0;
    size_t min_idx = 0;

    assert (NULL != arr);

    if (2 > size)
    {
        return;
    }

    for (i = 0; i < size - 1; ++i)
    {
        min_idx = i;
        for (j = i + 1; j < size; ++j)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

void InsertionSort(int *arr, size_t size)
{
    size_t i = 0;
    size_t j = 0;
    int key = 0;

    assert (NULL != arr);

    if (2 > size)
    {
        return;
    }

    for (i = 1; i < size; ++i)
    {
        key = arr[i];
        j = i;
        while (j > 0 && arr[j - 1] > key)
        {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}
