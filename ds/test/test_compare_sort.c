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

static int CmpQArr(int* arr1, int* arr2)
{
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			return 1;
		}
	}
	
	return 0;
}

static int compareInts(const void *a, const void *b) 
{
    return (*(int*)a - *(int*)b);
}

static void TestCountingSort(int *arr)
{	
	clock_t end = {0};
	clock_t start = clock();
	
	CountingSort(arr, ARR_SIZE);
	end = clock();
		printf("time is: " GREEN " %lf" RESET "\n", ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);	
	
	
	if (!CheckIfSorted(arr, ARR_SIZE))
	{
		printf("CountingSort" RED " failed" RESET "\n");	
		return;
	}
	
	printf("CountingSort" GREEN " passed" RESET "\n");	
}

static InitArr(int* arr, int max_out, int min_out)
{
	long min = min_out;
	long max = max_out;
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		arr[i] = (rand() % (max - min + 1)) + min;
	}
}

int main() 
{
	int* arr = (int*)malloc(sizeof(int) * ARR_SIZE);
	int* arr1 = (int*)malloc(sizeof(int) * ARR_SIZE);
	int* arr_copy = (int*)malloc(sizeof(int) * ARR_SIZE);
	int min_counting_sort = 1;
	int max_counting_sort = 100;
	clock_t end = {0};
	clock_t start = {0};
	
	
	InitArr(arr, INT_MAX, INT_MIN);
	CopyArr(arr, arr_copy);
	CopyArr(arr, arr1);
	

	start = clock();
	qsort(arr1, ARR_SIZE, sizeof(int), compareInts);
	end = clock();
		printf("qsort time is: " GREEN " %lf" RESET "\n", ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);	
	
	
	TestBubbleSort(arr);
	if (CmpQArr(arr, arr1))
	{
		printf("qsort vs bubbleSort" RED " failed" RESET "\n");	
		free(arr); arr = NULL;
		free(arr1); arr1 = NULL;
		free(arr_copy); arr_copy = NULL;
		return;
	}
	
	CopyArr(arr_copy, arr);
	TestSelectionSort(arr);
	if (CmpQArr(arr, arr1))
	{
		printf("qsort vs SelectionSort" RED " failed" RESET "\n");	
		free(arr); arr = NULL;
		free(arr1); arr1 = NULL;
		free(arr_copy); arr_copy = NULL;
		return;
	}
	
	CopyArr(arr_copy, arr);
	TestInsertionSort(arr);
	if (CmpQArr(arr, arr1))
	{
		printf("qsort vs InsertionSort" RED " failed" RESET "\n");	
		free(arr); arr = NULL;
		free(arr1); arr1 = NULL;
		free(arr_copy); arr_copy = NULL;
		return;
	}
	
	InitArr(arr, max_counting_sort, min_counting_sort);
	CopyArr(arr, arr1);
	TestCountingSort(arr);
	qsort(arr1, ARR_SIZE, sizeof(int), compareInts);
	if (CmpQArr(arr, arr1))
	{
		printf("qsort vs InsertionSort" RED " failed" RESET "\n");	
		free(arr); arr = NULL;
		free(arr1); arr1 = NULL;
		free(arr_copy); arr_copy = NULL;
		return;
	}
	
	free(arr); arr = NULL;
	free(arr1); arr1 = NULL;
	free(arr_copy); arr_copy = NULL;
	
    return 0;
}
