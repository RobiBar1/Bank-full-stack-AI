#include <stddef.h> /* size_t type */
#include <assert.h> /* for asserts use */
#include <stdlib.h> /* Required for malloc() and free() */


char* StrNCpy(char* dst, const char* src, size_t n_bytes)
{
	size_t i;
	
	assert(NULL != dst && NULL != src);
	
	for (i = 0; i < n_bytes && '\0' != src[i] && '\0' != dst[i]; i++)
	{
		dst[i] = src[i];
	}
	
	while (dst[i] != '\0')
	{
		dst[i] = '\0';
		i++;
	}
	
	return dst;
}
char* StrCpy(char* dst, char* src)
{
	size_t i;
	
	assert(NULL != dst && NULL != src);
	
	for (i = 0; '\0' != src[i] && '\0' != dst[i]; i++)
	{
		dst[i] = src[i];
	}
	
	while (dst[i] != '\0')
	{
		i++;
	}
	
	return &dst[i];
	
}


size_t StrLen(const char *str)
{
	const char* counter;
	size_t i;
	
	assert(NULL != str);
	
	counter = str;
	for (i = 0; '\0' != str[i]; ++i)
	{
		counter++;
	}
	
	return counter - str;
}

int StrCmp(const char *s1, const char *s2)
{
	size_t i;
	assert(NULL != s1 && NULL != s2);
	
	for (i = 0; '\0' != s1[i] && '\0' != s2[i]; ++i)
	{
		if (s1[i] == s2[i])
		{
			continue;
		}
		if (s1[i] < s2[i])
		{
			return -1;
		}
		
		return 1;
	}
	
	if ('\0' != s1[i])
	{
		return 1;
	}
	if ('\0' != s2[i])
	{
		return -1;
	}
	
	return 0;
}


