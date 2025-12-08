#include <assert.h>    /* for asserts using */
#include <stdio.h>     /* for using prints */
#include <string.h>    /* for strlen, strcmp, etc.. */
#include <stdlib.h>    /* for malloc alocate */
#include "pointers2.h" /* for our functions */


void Print_Test_Start(char* func_name);
void Print_Test_End(char* func_name);
void resetArray(int* arr, size_t size);
void printArray(int* arr, size_t size);

void test_ISPalindrome(void);

int main()
{
	test_ISPalindrome();
	
	return 0;
}

void test_ISPalindrome(void)
{
	const char even_poli[] = "heyyeh"; 
	const char odd_poli[] = "heyeh"; 
	const char non_even_poli[] = "heyyah";
	const char non_odd_poli[] = "has";
	
	Print_Test_Start("ISPalindrome");
	if (1 != ISPalindrome(even_poli))
	{
		printf("ISPalindrome Failed with even_poli input");
		return;
	}
	
	if (1 != ISPalindrome(odd_poli))
	{
		printf("ISPalindrome Failed with odd_poli input");
		return;
	}
	
	if (0 != ISPalindrome(non_even_poli))
	{
		printf("ISPalindrome Failed with non_even_poli input");
		return;
	}
	
	if (0 != ISPalindrome(non_odd_poli))
	{
		printf("ISPalindrome Failed with non_odd_poli input");
		return;
	}
	
	Print_Test_End("ISPalindrome");
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
