#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "compare_sort.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

#define ARR_SIZE 100000

#define NOT_SORTED (0)
#define SORTED (1)

static CheckIfSorted(int* arr, size_t size)
{
	size_t i = 0;
	
	for (; i < ARR_SIZE - 1; ++i)
	{
		if (arr[i] > arr[i + 1])
		{
			return NOT_SORTED;
		}
	}
	
	return SORTED;
}

static InitArr(int* arr)
{
	long min = INT_MIN;
	long max = INT_MAX;
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		arr[i] = (rand() % (max - min + 1)) + min;
	}
}

static void CopyArr(int* from, int* to)
{
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		to[i] = from[i];
	}
}
static void TestInsertionSort(int* arr)
{
	clock_t end = {0};
	clock_t start = clock();
	
	InsertionSort(arr, ARR_SIZE);	
	end = clock();
	
	printf("time is: " GREEN " %lf" RESET "\n", ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);	
	if (!CheckIfSorted(arr, ARR_SIZE))
	{
		printf("InsertionSort" RED " failed" RESET "\n");	
		return;
	}
	
	printf("InsertionSort" GREEN " passed" RESET "\n");	
}

static void TestSelectionSort(int* arr)
{

	clock_t end = {0};
	clock_t start = clock();
	
	SelectionSort(arr, ARR_SIZE);	
	end = clock();
		printf("time is: " GREEN " %lf" RESET "\n", ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);	
	if (!CheckIfSorted(arr, ARR_SIZE))
	{
		printf("SelectionSort" RED " failed" RESET "\n");	
		return;
	}
	
	printf("SelectionSort" GREEN " passed" RESET "\n");	
}

static void TestBubbleSort(int *arr)
{	
	clock_t end = {0};
	clock_t start = clock();
	
	BubbleSort(arr, ARR_SIZE);
	end = clock();
		printf("time is: " GREEN " %lf" RESET "\n", ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);	
	
	
	if (!CheckIfSorted(arr, ARR_SIZE))
	{
		printf("bubbleSort" RED " failed" RESET "\n");	
		return;
	}
	
	printf("bubbleSort" GREEN " passed" RESET "\n");	
}


int main() 
{
	int* arr = (int*)malloc(sizeof(int) * ARR_SIZE);
	int* arr_copy = (int*)malloc(sizeof(int) * ARR_SIZE);
	
	InitArr(arr);
	CopyArr(arr, arr_copy);
	
	TestBubbleSort(arr);
	
	CopyArr(arr_copy, arr);
	TestSelectionSort(arr);
	
	CopyArr(arr_copy, arr);
	TestInsertionSort(arr);
	
    return 0;
}
