#include <assert.h> /* for using asserts */
#include "String.h" /* for using StrLen */

static int IsHave7(long num)
{
	if (num < 0)
	{
		num *= -1;
	}
	
	while (num > 0)
	{
		if (7 == num % 10)
		{
			return 1;
		}
		
		num /= 10;
	}
	
	return 0;
}

static void SevenBoomOutPut(long from)
{
	if (0 == from % 7 || IsHave7(from))
	{
		printf("boom\n");
	}
	else
	{
		printf("%ld\n", from);
	}
}
void SevenBoom(long from, long to)
{	
	while (from < to)
	{
		SevenBoomOutPut(from);
		++from;
	}
	while (from > to)
	{
		SevenBoomOutPut(from);
		--from;
	}
	
	SevenBoomOutPut(from); /* for the equal */
}

static int IsWhiteSpace(int c)
{
	return (' ' == c || '\t' == c);
}

char* RemoveWhiteSpaces(char* str)
{
	char* start = NULL, *ruuner = NULL;
	
	assert(NULL != str);
	
	start = str;
	ruuner = str;
	/* for prefix whitespaces, e.x:    hey -> in the end will point to 'h' */
	while (*str && IsWhiteSpace(*str++));
	
	if(!*str)
	{
		return NULL;
	}
	
	do
	{
		*ruuner++ = *str;
	} while (*str && IsWhiteSpace(*++str));
	
	if(!IsWhiteSpace(*ruuner))
	{
		++start;
	}
	
	*ruuner = '/0';
	
	return start;
	
}

int ISPalindrome(const char* str)
{
	size_t i = 0, j = 0;
	
	assert(NULL != str);
	
	j = StrLen(str) - 1;
	if (j < 1)
	{
		return 1;
	}
	
	for (; i <= j; ++i, --j)
	{
		if (str[i] != str[j])
		{
			return 0;
		}
	}
	
	return 1;
}
