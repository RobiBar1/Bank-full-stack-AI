#include <string.h>
#include <assert.h>
#include <stdio.h>

static void Swap(char* left, char* right)
{
	char tmp = 0;
	
	assert (NULL != left);
	assert (NULL != right);
	
	tmp = *left;
	*left = *right;
	*right = tmp;
}

static void PrintAndMoveCharToEnd(char** str, size_t size)
{
	size_t i = 0;
	
	assert (NULL != str);
	assert (NULL != *str);
	
	printf("%s\n", *str);
	for (; i < size - 1; ++i)
	{
		Swap(&((*str)[i]), &((*str)[i + 1]));
		printf("%s\n", *str);
	}
	
	Swap(&((*str)[0]), &((*str)[1]));
}

static void PrintPermutations(char* str)
{
	size_t i = 0;
	size_t size = 0;
	
	assert (NULL != str);
	
	size = strlen(str);
	for (; i < size - 1; ++i)
	{
		PrintAndMoveCharToEnd(&str, size);
	}
}

int main()
{
	char str[4] = "abc";
	char str1[5] = "abcd";
	char str2[6] = "abcde";
	
	PrintPermutations(str);
	printf("=======================================\n");
	PrintPermutations(str1);
	printf("=======================================\n");
	PrintPermutations(str2);
	printf("=======================================\n");
		
	return 0;
}
