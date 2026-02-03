/*
Writer:  Robi
Checker: Roie
Date:    01.02.2026
*/

#include <assert.h> /* assert   */
#include <stddef.h> /* malloc   */
#include <stdlib.h> /* calloc   */
#include <stdint.h> /* SIZE_MAX */
#include "compare_sort.h"

#define COUNTING_RANGE 100
#define RADIX_COUNTING_RANGE 10

#define SUCCESS 0
#define NOT_SUCCESS -1

#define NONE_NEGETIVE_INDEX(x) ((x) - 1) != SIZE_MAX ? ((x) - 1) : 0
#define BASE(x) x
#define GET_NUM(num, devide_by) ((num) / (devide_by)) % BASE(10)

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
	int temp = 0;
	
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
                /*swap(&arr[j], &arr[j + 1]);*/
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
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
    int temp = 0;

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
            /*swap(&arr[i], &arr[min_idx]);*/
             temp = arr[i];
             arr[i] = arr[min_idx];
             arr[min_idx] = temp;
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

int CountingSort(int* arr, size_t size)
{
	size_t i = 0;
	size_t* counting_arr;
	size_t* new_arr;
	size_t val = 0;
	
	assert (NULL != arr);
	
	counting_arr = (size_t*)calloc(1, 
						sizeof(size_t) * (COUNTING_RANGE + 1));
	if (NULL == counting_arr)
	{
		return NOT_SUCCESS;
	}
	new_arr = (size_t*)malloc(sizeof(size_t) * size);
	if (NULL == new_arr)
	{
		free(counting_arr); counting_arr = NULL;
		return NOT_SUCCESS;
	}
	
	for (; i < size; ++i)
	{
		++counting_arr[arr[i]];
	}
	
	for (i = 1; i < COUNTING_RANGE + 1; ++i)
	{
		counting_arr[i] += counting_arr[i - 1];
	}
	
	for (i = size - 1; i != SIZE_MAX ; --i)
	{
		counting_arr[arr[i]] = NONE_NEGETIVE_INDEX(counting_arr[arr[i]]);
		val = counting_arr[arr[i]];
		new_arr[val] = arr[i];
	}
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = new_arr[i];
	}
	
	free(counting_arr); counting_arr = NULL;
	free(new_arr); new_arr = NULL;	
	
	return SUCCESS;	
}

int RadixCountingSort(int* arr, size_t size, size_t devide_by)
{
	size_t i = 0;
	size_t counting_arr[RADIX_COUNTING_RANGE] = {0};
	size_t* new_arr = NULL;
	size_t val = 0;
	
	assert (NULL != arr);
	
	new_arr = (size_t*)malloc(sizeof(size_t) * size);
	if (NULL == new_arr)
	{
		return NOT_SUCCESS;
	}
	
	for (; i < size; ++i)
	{
		++(counting_arr[GET_NUM(arr[i], devide_by)]);
	}
	
	for (i = 1; i < RADIX_COUNTING_RANGE; ++i)
	{
		counting_arr[i] += counting_arr[i - 1];
	}
	
	for (i = size - 1; i != SIZE_MAX ; --i)
	{
		val = --counting_arr[GET_NUM(arr[i], devide_by)];
		new_arr[val] = arr[i];
	}
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = new_arr[i];
	}
	
	free(new_arr); new_arr = NULL;	
	
	return SUCCESS;
}

int RadixSort(int* arr, size_t size)
{
	int max = 0;
	size_t i = 0;
	
	assert (NULL != arr);
	
	max = arr[i++];
	for (; i < size; ++i)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}
	
	for (i = 1; (max / i) > 0; i *= BASE(10))
	{
		if (NOT_SUCCESS == RadixCountingSort(arr, size, i))
		{
			return NOT_SUCCESS;
		}
	}
	
	return SUCCESS;	
}
