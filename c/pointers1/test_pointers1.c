#include <assert.h>    /* for asserts using */
#include <stdio.h>     /* for using prints */
#include <stdlib.h>    /* for using malloc */
#include "pointers1.h" /* for functions to test */

void TestSwapInt();
void TestCopyArray();
void TestPrintTypesAddress(void);

void Print_Test_Start(char* func_name);
void Print_Test_End(char* func_name);
void resetArray(int* arr, size_t size);
void printArray(int* arr, size_t size);




int main()
{
	TestSwapInt();	
	TestCopyArray();
	TestPrintTypesAddress();
	
	return 0;
}

void TestSwapInt()
{
	int x = 3, y = 10;
	
	Print_Test_Start("SwapInt");
	SwapInt(&x, &y);
	assert( 10 == x && 3 == y);
	/*
	worked:
	SwapInt(NULL, &y);
	assert( 10 == x && 3 == y);
	SwapInt(&x, NULL);
	assert( 10 == x && 3 == y);
	*/
	Print_Test_End("SwapInt");
}

void TestCopyArray()
{
	size_t size = 7, i;
	int* arr1 = (int*)malloc(sizeof(int) * size);
	int* arr2 = (int*)malloc(sizeof(int) * size);
	
	for (i = 0; i < size; i++)
	{
		arr1[i] = (i + 1);
		arr2[i] = (-1);
	} 
	
	if (NULL == arr2 || NULL == arr1)
	{
		printf("fail alocate memory in test copry array");
	}
	
	Print_Test_Start("CopyArray");
	CopyArray(arr1, arr2, size);
	for (i = 0; i < size; ++i)
	{
		assert(arr1[i] == arr2[i]);	
	}
	
	printf("first work\n");
	resetArray(arr2, size);
	CopyArray(NULL, arr2, size);
	for (i = 0; i < size; ++i)
	{
		assert(0 == arr2[i]);	
	}
	
	printf("second work\n");
	CopyArray(arr1, NULL, size);
	for (i = 0; i < size; ++i)
	{
		assert(0 == arr2[i]);	
	}
	
	printf("third work\n");
	size = 0;
	CopyArray(arr1, arr2, size);
	size = 7;
	for (i = 0; i < size; ++i)
	{
		assert(0 == arr2[i]);	
	}
	
	Print_Test_End("CopyArray");
}

void TestPrintTypesAddress(void)
{
	Print_Test_Start("PrintTypesAddress");
	PrintTypesAddress();
	Print_Test_End("PrintTypesAddress");
}

void resetArray(int* arr, size_t size)
{
	size_t i;
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = 0;	
	}
}

void printArray(int* arr, size_t size)
{
	size_t i;
	
	for(i = 0; i < size; ++i)
	{
		printf("%d, ", arr[i]);
	}
	
	printf("\n");
}

void Print_Test_Start(char* func_name)
{
	printf("--------------------------------------------------------------\n");
	printf("start testing for: \"%s()\" \n", func_name);	
}
void Print_Test_End(char* func_name)
{
	printf("Done SECCSFUL testing for: \"%s()\" \n", func_name);
	printf("--------------------------------------------------------------\n");	
}
