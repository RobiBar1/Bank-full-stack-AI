#include <assert.h> /* for using asserts */
#include <stdio.h>  /* for printf */

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
    char* read = str;
    char* write = str;
    char* last_char_saw = NULL;
    

    assert(NULL != str);

    while (*read && IsWhiteSpace(*read))
    {
    	++read;
    }
    while (*read)
    {
    	if (IsWhiteSpace(*read))
    	{
			*write++ = *read++;
			while (IsWhiteSpace(*read))
			{
				read++;
			}
    	}
    	else
    	{	
    		last_char_saw = write;
    		*write++ = *read++;
    	}
    }
	
	if (last_char_saw)
	{
		++last_char_saw;
		*last_char_saw = '\0';
	}
	
     *write = '\0';

    return str;
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
