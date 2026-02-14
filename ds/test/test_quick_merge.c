#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "quick_merge.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"
#define ARR_SIZE 5000

#define NOT_SORTED (0)
#define SORTED (1)

static void TestBinarySerchIter(char* func_name)
{
    int arr[] = {1, 3, 5, 6, 7, 8, 10, 15, 30, 33, 36, 39, 51, 53};
    int arr2[] = {1};
    size_t size = sizeof(arr) / sizeof(int);
    size_t size2 = sizeof(arr2) / sizeof(int);
    int num_to_find = 7;
    int expted_answer = 4;
    int real_answer = 0;

    real_answer = BinarySerchIterative(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 36;
    expted_answer = 10;
    real_answer = BinarySerchIterative(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 12;
    expted_answer = -1;
    real_answer = BinarySerchIterative(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 1;
    expted_answer = 0;
    real_answer = BinarySerchIterative(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 0;
    expted_answer = -1;
    real_answer = BinarySerchIterative(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    printf(GREEN "SUCCESS on test %s\n", func_name);
    printf("=================================================================\n" RESET);
}

static void TestBinarySerchRecursive(char* func_name)
{
    int arr[] = {1, 3, 5, 6, 7, 8, 10, 15, 30, 33, 36, 39, 51, 53};
    int arr2[] = {1};
    size_t size = sizeof(arr) / sizeof(int);
    size_t size2 = sizeof(arr2) / sizeof(int);
    int num_to_find = 7;
    int expted_answer = 4;
    int real_answer = 0;

    real_answer = BinarySerchRecursive(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 36;
    expted_answer = 10;
    real_answer = BinarySerchRecursive(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 12;
    expted_answer = -1;
    real_answer = BinarySerchRecursive(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 1;
    expted_answer = 0;
    real_answer = BinarySerchRecursive(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 0;
    expted_answer = -1;
    real_answer = BinarySerchRecursive(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    printf(GREEN "SUCCESS on test %s\n", func_name);
    printf("=================================================================\n" RESET);
}


static int compareInts(const void *a, const void *b) 
{
    return (*(int*)a - *(int*)b);
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

static void CopyArr(int* from, int* to)
{
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		to[i] = from[i];
	}
}

static void InitArr(int* arr, int max, int min)
{
	int i = 0;
    long max_long = max;
    long min_long = min;
	srand(time(NULL));
	
	for (; i < ARR_SIZE; ++i)
	{
		arr[i] = (rand() % (max_long - min_long + 1) + min_long);
	}
}

static int TestMergeSort(char* func_name, int* arr, int* arr1)
{
    clock_t end = {0};
	clock_t start = {0};

    printf("===================== Started test %s ===================\n", func_name);
     
    start = clock();
	qsort(arr1, ARR_SIZE, sizeof(int), compareInts);
	end = clock();
	printf("qsort time is: " GREEN " %f\n" RESET, ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);
	
    start = clock();
	MergeSortRecursive(arr, ARR_SIZE);
    end = clock();
    printf("%s time is: " GREEN " %f\n" RESET, func_name, 
                            ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);

	if (CmpQArr(arr, arr1))
	{
		printf("qsort vs %s" RED "failed\n" RESET, func_name);	
		return 1 ;
	}

    printf(GREEN "SUCCESS on test %s\n", func_name);
    printf("=================================================================\n" RESET);

    return 0;
}

static int TestQuickSort(char* func_name, int* arr, int* arr1)
{
    clock_t end = {0};
	clock_t start = {0};

    printf("===================== Started test %s ===================\n", func_name);
	
    if (!CmpQArr(arr, arr1))
    {
        printf(RED "need init arr in function %s\n" RESET, func_name);
        return 1;	
    }

    start = clock();
	QuickSortRecursive(arr, ARR_SIZE, sizeof(int), compareInts);
    end = clock();
    printf("%s time is:" GREEN " %f\n" RESET, func_name, 
                            ((double)(end) - (double)(start)) / CLOCKS_PER_SEC);

	if (CmpQArr(arr, arr1))
	{
		printf("qsort vs %s" RED "failed\n" RESET, func_name);	
		return 1 ;
	}

    printf(GREEN "SUCCESS on test %s\n", func_name);
    printf("=================================================================\n" RESET);

    return 0;
}

int main() 
{
    int* arr = (int*)malloc(sizeof(int) * ARR_SIZE);
	int* arr1 = (int*)malloc(sizeof(int) * ARR_SIZE);
	int* arr_copy = (int*)malloc(sizeof(int) * ARR_SIZE);

    InitArr(arr, INT_MAX, INT_MIN);
	CopyArr(arr, arr_copy);
	CopyArr(arr, arr1);

    TestBinarySerchIter("BinarySerchIterative");
    TestBinarySerchRecursive("BinarySerchRecursive");
    if (TestMergeSort("MergeSortRecursive", arr, arr1))
    {
        free(arr); arr = NULL;
		free(arr1); arr1 = NULL;
		free(arr_copy); arr_copy = NULL;
        return 0;
    }

    CopyArr(arr_copy, arr);
    if (TestQuickSort("QuickSortRecursive", arr, arr1))
    {
        free(arr); arr = NULL;
		free(arr1); arr1 = NULL;
		free(arr_copy); arr_copy = NULL;
        return 0;
    }


    free(arr); arr = NULL;
    free(arr1); arr1 = NULL;
    free(arr_copy); arr_copy = NULL;
    return 0;
}
