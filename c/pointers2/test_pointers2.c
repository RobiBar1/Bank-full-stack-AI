#include <assert.h>    /* for asserts using */
#include <stdio.h>     /* for using prints */
#include <string.h>    /* for strlen, strcmp, etc.. */
#include <stdlib.h>    /* for malloc alocate */
# include <strings.h>  

#include "pointers2.h" /* for our functions */


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"


void Print_Test_Start(char* func_name);
void Print_Test_End(char* func_name);
void resetArray(int* arr, size_t size);
void printArray(int* arr, size_t size);

void test_ISPalindrome(void);

static int TestQ2SevenBoom();
static int TestQ3RemoveWhiteSpace_Basic();
static int TestQ3RemoveWhiteSpace_WithTabs();
static int TestQ3RemoveWhiteSpace_Empty();
static int TestQ3RemoveWhiteSpace_JustSpacesAndTabs();
static int TestQ3RemoveWhiteSpace_JustSpaceAtStart();
static void CheckTest(int result, char* name);

int main()
{
	test_ISPalindrome();
	
	printf("---- TEST Q2 ----\n");
	CheckTest(TestQ2SevenBoom(), "TestQ2SevenBoom()");
	
	printf("---- TEST Q2 ----\n");
	CheckTest(TestQ3RemoveWhiteSpace_Basic(), "TestQ3RemoveWhiteSpace_Basic()");
	CheckTest(TestQ3RemoveWhiteSpace_WithTabs(), "TestQ3RemoveWhiteSpace_WithTabs()");
	CheckTest(TestQ3RemoveWhiteSpace_Empty(), "TestQ3RemoveWhiteSpace_Empty()");
	CheckTest(TestQ3RemoveWhiteSpace_JustSpacesAndTabs(),
	"TestQ3RemoveWhiteSpace_JustSpacesAndTabs()");
	CheckTest(TestQ3RemoveWhiteSpace_JustSpaceAtStart(), "TestQ3RemoveWhiteSpace_JustSpaceAtStart()");

	return 0;
}

static void CheckTest(int result, char* name)
{
	if (result == 0)
	{
		printf("-> %s: ", name);
		printf(GREEN "PASSED\n" RESET);
	}
	else 
	{
		printf(RED "-> %s: FAILED\n" RESET, name);
	}
}

static int TestQ2SevenBoom()
{
	SevenBoom(-14, 30);
	return 0;
}

static int TestQ3RemoveWhiteSpace_Basic()
{
	char str[] = "   hey  apple   ";
	RemoveWhiteSpaces(str);
	printf("%s\n", str);
	return !(0 == strcmp(str, "hey apple"));
}

static int TestQ3RemoveWhiteSpace_WithTabs()
{
	char str[] = " hey  apple		added tab  ";
	RemoveWhiteSpaces(str);
	printf("%s\n", str);
	return !(0 == strcmp(str, "hey apple	added tab"));
}

static int TestQ3RemoveWhiteSpace_Empty()
{
	char str[] = "";
	RemoveWhiteSpaces(str);
	printf("%s\n", str);
	return !(0 == strcmp(str, ""));
}

static int TestQ3RemoveWhiteSpace_JustSpacesAndTabs()
{
	char str[] = " 	 	 ";
	RemoveWhiteSpaces(str);
	printf("%s\n", str);
	return !(0 == strcmp(str, ""));
}

static int TestQ3RemoveWhiteSpace_JustSpaceAtStart()
{
	char str[] = " apple";
	RemoveWhiteSpaces(str);
	printf("%s\n", str);
	return !(0 == strcmp(str, "apple"));
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
