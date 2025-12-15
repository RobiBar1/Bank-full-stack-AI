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
void test_StrCaseCmp(void);
void test_StrChr(void);

void test_StrTok(void);
void test_StrSpn(void);
void test_StrStr(void);
void test_StrCat(void);
void test_StrNCat(void);
void test_StrDup(void);

int main()
{
	test_StrLen();
	test_StrCmp();
	test_StrCpy();
	test_StrNCpy();
	test_StrCaseCmp();
	
    test_StrDup();
    test_StrCat();
    test_StrNCat();
    test_StrStr();
    test_StrSpn();
    test_StrChr();
    test_StrTok();
    
	return 0;
}

void test_StrTok(void)
{
    char str[] = "One,Two,Three"; 
    char delims[] = ",";
    char* token = NULL;

    Print_Test_Start("StrTok");
    token = StrTok(str, delims);
    if (NULL == token || 0 != strcmp(token, "One"))
    {
        printf("fail test StrTok: 1st token. Expected 'One', Got '%s'\n", token ? token : "NULL");
        return;
    }

    token = StrTok(NULL, delims);    
    if (NULL == token || 0 != strcmp(token, "Two"))
    {
        printf("fail test StrTok: 2nd token. Expected 'Two', Got '%s'\n", token ? token : "NULL");
        return;
    }

    token = StrTok(NULL, delims);    
    if (NULL == token || 0 != strcmp(token, "Three"))
    {
        printf("fail test StrTok: 3rd token. Expected 'Three', Got '%s'\n", token ? token : "NULL");
        return;
    }

    token = StrTok(NULL, delims);
    if (NULL != token)
    {
        printf("fail test StrTok: Expected NULL at end, but got '%s'\n", token);
        return;
    }

    Print_Test_End("StrTok");
}
void test_StrDup(void)
{
    char source[] = "DuplicateMe";
    char* copy = NULL;

    Print_Test_Start("StrDup");
    copy = StrDup(source);

    if (NULL == copy)
    {
        printf("fail test StrDup: returned NULL\n");
        return;
    }

    if (0 != strcmp(copy, source))
    {
        printf("fail test StrDup: strings are not equal\n");
        printf("Expected: %s, Got: %s\n", source, copy);
        free(copy); copy = NULL;
        return;
    }
    
    /* valid that deep copy(by memory) */
    if (copy == source)
    {
        printf("fail test StrDup: pointers are identical (shallow copy)\n");
        return;
    }

    free(copy); copy = NULL;
    Print_Test_End("StrDup");
}
    
void test_StrCat(void)
{
    char s1[20] = "Hello"; 
    char s2[] = "World";
    char* result = NULL;
    
    Print_Test_Start("StrCat");
    result = StrCat(s1, s2);
    if (NULL == result)
    {
        printf("fail test StrCat: returned NULL\n");
        return;
    }

    if (0 != strcmp(result, "HelloWorld"))
    {
        printf("fail test StrCat: result incorrect\n");
        printf("Expected: HelloWorld, Got: %s\n", result);
        return;
    }

    Print_Test_End("StrCat");
}

void test_StrNCat(void)
{
    char s1[20] = "Hello";
    char s2[] = "World";
    char* result = NULL;
    
    Print_Test_Start("StrNCat");
    result = StrNCat(s1, s2, 3);
    if (NULL == result)
    {
        printf("fail test StrNCat: returned NULL\n");
        return;
    }

    if (0 != strcmp(result, "HelloWor"))
    {
        printf("fail test StrNCat: result incorrect\n");
        printf("Expected: HelloWor, Got: %s\n", result);
        return;
    }

    Print_Test_End("StrNCat");
}

void test_StrStr(void)
{
    char haystack[] = "Simple String Search";
    char needle[] = "String";
    char* result = NULL;;

    Print_Test_Start("StrStr");
    result = StrStr(haystack, needle);
    if (NULL == result)
    {
        printf("fail test StrStr: needle not found\n");
        return;
    }

    if (0 != strncmp(result, needle, strlen(needle)))
    {
        printf("fail test StrStr: found incorrect location\n");
        return;
    }
	result = StrStr(haystack, "Zebra");
    if (NULL != result)
    {
        printf("fail test StrStr: found non-existent string: %s\n", result);
        return;
    }

    Print_Test_End("StrStr");
}

void test_StrSpn(void)
{
    char str[] = "12345abcde";
    char accept[] = "1234567890";
    size_t len = 0;

    Print_Test_Start("StrSpn");
    len = StrSpn(str, accept);
    if (5 != len)
    {
        printf("fail test StrSpn: incorrect length\n");
        printf("Expected: 5, Got: %lu\n", (unsigned long)len);
        return;
    }

    len = StrSpn(str, "xyz");
    if (0 != len)
    {
        printf("fail test StrSpn: found matches where none exist\n");
        return;
    }

    Print_Test_End("StrSpn");
}

void test_StrChr(void)
{
	char arrHello[] = "Hello";
	
	Print_Test_Start("StrChr");
	if(StrChr(arrHello, arrHello[2]) != &arrHello[2])
	{
		printf("fail test StrChr for StrChr(arrHello, arrHello[2]) compere\n");
		return;
	}
	
	if(NULL != StrChr(arrHello, 'a'))
	{
		printf("fail test StrChr for input: StrChr(arrHello, 'a') compere\n");
		return;
	}
	
	Print_Test_End("StrChr");
}

void test_StrCaseCmp()
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
	
	Print_Test_Start("StrCaseCmp");
	if ( 0 != StrCaseCmp(arr1, arr2))
	{
		printf("fail test StrCaseCmp for first compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	arr1[3] = 'a';
	arr2[3] = 'A';
	if ( 0 != StrCaseCmp(arr1, arr2))
	{
		printf("fail test StrCaseCmp for second compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	arr2[3] = 'b';
	if ( -1 != StrCaseCmp(arr1, arr2))
	{
		printf("fail test StrCaseCmp for negetive compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	arr1[3] = 'z';
	if ( 1 != StrCaseCmp(arr1, arr2))
	{
		printf("fail test StrCaseCmp for positive compere\n");
		free(arr1);
		free(arr2);
		return;
	}
	
	Print_Test_End("StrCaseCmp");
	
	free(arr1);
	free(arr2);

}

void test_StrNCmp(void)
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
	if ( 0 != StrNCmp(arr1, arr2, 7))
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
		free(arr2); arr2 = NULL;
		return;
	}

	free(arr2); arr2 = NULL;
	Print_Test_End("StrNCpy");
}


void test_StrCpy(void)
{	
	char arrHello[] = "Hello";
	size_t i, size = 6;
	char* arr1 = (char*)malloc(sizeof(char) * size);
	char* arr2 = (char*)malloc(sizeof(char) * size);
	
	Print_Test_Start("StrCpy");
	if(NULL == arr1 || NULL == arr2)
	{
		printf("fail alocate memory\n");
		return;
	}
	
	for (i = 0; i < size; ++i)
	{
		arr1[i] = arrHello[i];
	}
	
	StrCpy(arr2, arr1);
	if ( 0 != strcmp(arr2, arr1))
	{
		printf("fail test StrCpy for first copy\n");
		printArray( arr1, size);
		printArray( arr2, size);
		free(arr1); arr1 = NULL;
		free(arr2); arr2 = NULL;
		return;
	}
	
	free(arr1); arr1 = NULL;
	free(arr2); arr2 = NULL;
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
