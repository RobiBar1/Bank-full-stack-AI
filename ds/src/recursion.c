#include <string.h> 	/* strncmp */
#include <assert.h> 	/* assert  */

#include "recursion.h"  /* our api */

stack_t* SortStack(stack_t* stack);

size_t Strlen(const char* str)
{
	assert (NULL != str);
	
	if ('\0' == *str)
	{
		return 0;
	}
	
	return 1 + Strlen(++str);
}

int StrCmp(const char *s1, const char *s2)
{
	assert (NULL != s1);
	assert (NULL != s2);
	
	if ('\0' == *s1 || *s1 != *s2)
	{
		return *s1 - *s2;
	}
	
	return StrCmp(++s1, ++s2);
}

char* StrCpy(char* dst, const char* src)
{
	assert (NULL != dst);
	assert (NULL != src);
	
	*dst = *src;
	if ('\0' == *src)
	{		
		return dst;
	}
	
	StrCpy(dst + 1, src + 1);
	
	return dst;
}

char* StrCat(char* dst, const char* src)
{
	char* tmp = dst;
	
	assert (NULL != dst);
	assert (NULL != src);
	
	dst = dst + Strlen(dst);
	StrCpy(dst, src);
	
	return tmp;
}

char* StrStr(const char* haystack, const char* needle)
{
	assert (NULL != haystack);
	assert (NULL != needle);
	
	if ('\0' == *haystack)
	{
		return NULL;
	}
	
	if (0 == strncmp(haystack, needle, Strlen(needle)))
	{
		return (char*)haystack;
	}
	
	return StrStr(haystack + 1, needle);
}

int FibonacciRecursion(int element_index)
{
	if (0 == element_index || 1 == element_index)					
	{
		return element_index;
	}
	
	return (FibonacciRecursion(element_index - 1) +
		   FibonacciRecursion(element_index - 2));
}

int FibonacciIteratively(int element_index)
{
	int a0 = 0;
	int sum = 0;
	int a1 = 1;
	int i = 2;
	
	if (0 == element_index || 1 == element_index)					
	{
		return element_index;
	}
	
	for (; i <= element_index; ++i)
	{
		sum = a0 + a1;
		a0 = a1;
		a1 = sum;
	}
	
	return sum;
}

sll_node_t* FlipList(sll_node_t* head)
{
																						
}
