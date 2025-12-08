#include <assert.h>    /* for asserts using */
#include <stdio.h>     /* for using prints */
#include <string.h>    /* for strlen, strcmp, etc.. */
#include <stdlib.h>    /* for malloc alocate */
#include "String.h" /* for our functions */


void Print_Test_Start(char* func_name);
void Print_Test_End(char* func_name);
void resetArray(int* arr, size_t size);
void printArray(const char* arr, size_t size);


void test_StrLen(void);
void test_StrCmp(void);
void test_StrNCmp(void);
void test_StrCpy(void);
void test_StrNCpy(void);


int main()
{
	test_StrLen();
	test_StrCmp();
	test_StrCpy();
	test_StrNCpy();
	return 0;
}

void test_StrNCmp(void)
{
	char arrHello[] = "Hello";
	size_t i, size = 6, n_to_check = size - 4;
	char* arr1 = (char*)malloc(sizeof(char) * size);
	char* arr2 = (char*)malloc(sizeof(char) * size);
	
	assert(NULL != arr1 && NULL != arr2);
	
	for (i = 0; i < size; ++i)
	{
		arr1[i] = arrHello[i];
		arr2[i] = arrHello[i];
	}
	
	arr2[3] = arr2[3] + 1;
	Print_Test_Start("StrNCmp");
	if ( 0 != StrNCmp(arr1, arr2, 3))
	{
		printf("fail test StrNCmp for first compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	if ( -1 != StrNCmp(arr1, arr2, 4))
	{
		printf("fail test StrNCmp for negetive compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	arr1[3] = arr2[3];
	if ( 0 != StrCmp(arr1, arr2, 7))
	{
		printf("fail test StrNCmp for last compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	Print_Test_End("StrNCmp");
	
	free(arr1);
	free(arr2);
	
}
void test_StrNCpy(void)
{
	const char arr1[] = "Hello";
	size_t i, size = 6;
	char* arr2 = (char*)malloc(sizeof(char) * size);
	
	assert(NULL != arr1 && NULL != arr2);
	
	for (i = 0; i < size; ++i)
	{
		arr2[i] = arr1[i];
	}
	
	Print_Test_Start("StrNCpy");
	StrNCpy(arr2, arr1, size);	
	if ( 0 != strcmp(arr2, arr1))
	{
		printf("fail test StrCpy for first copy\n");
		printArray( arr1, size);
		printArray( arr2, size);
		free(arr2);
		return;
	}

	free(arr2);
	Print_Test_End("StrNCpy");
}


void test_StrCpy(void)
{	
	char arrHello[] = "Hello";
	size_t i, size = 6;
	char* arr1 = (char*)malloc(sizeof(char) * size);
	char* arr2 = (char*)malloc(sizeof(char) * size);
	
	assert(NULL != arr1 && NULL != arr2);
	
	for (i = 0; i < size; ++i)
	{
		arr1[i] = arrHello[i];
	}
	
	Print_Test_Start("StrCpy");
	StrCpy(arr2, arr1);
	if ( 0 != strcmp(arr2, arr1))
	{
		printf("fail test StrCpy for first copy\n");
		printArray( arr1, size);
		printArray( arr2, size);
		free(arr1);
		free(arr2);
		return;
	}
	
	Print_Test_End("StrCpy");
}

void test_StrCmp(void)
{
	char arrHello[] = "Hello";
	size_t i, size = 6;
	char* arr1 = (char*)malloc(sizeof(char) * size);
	char* arr2 = (char*)malloc(sizeof(char) * size);
	
	assert(NULL != arr1 && NULL != arr2);
	
	for (i = 0; i < size; ++i)
	{
		arr1[i] = arrHello[i];
		arr2[i] = arrHello[i];
	}
	
	Print_Test_Start("StrCmp");
	if ( 0 != StrCmp(arr1, arr2))
	{
		printf("fail test StrCmp for first compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	arr1[3] = 'a';
	if ( -1 != StrCmp(arr1, arr2))
	{
		printf("fail test StrCmp for negetive compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	arr1[3] = 'z';
	if ( 1 != StrCmp(arr1, arr2))
	{
		printf("fail test StrCmp for positive compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	Print_Test_End("StrCmp");
	
	free(arr1);
	free(arr2);
}


void test_StrLen(void)
{
	char arrHello[] = "Hello";
	size_t i, size = 6;
	char* arr1 = (char*)malloc(sizeof(char) * size);
	char* arr2 = (char*)malloc(sizeof(char) * size);
	
	assert(NULL != arr1 && NULL != arr2);
	
	for (i = 0; i < size; ++i)
	{
		arr1[i] = arrHello[i];
		arr2[i] = arrHello[i];
	}
	
	Print_Test_Start("StrLen");
	if (5 != StrLen(arr1))
	{
		printf("fail test strLen with with promt arr1\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	free(arr1);
	free(arr2);
	Print_Test_End("StrLen");
}







void resetArray(int* arr, size_t size)
{
	size_t i;
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = 0;	
	}
}

void printArray(const char* arr, size_t size)
{
	size_t i;
	
	for(i = 0; i < size; ++i)
	{
		printf("%c, ", arr[i]);
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
