#include <assert.h> /* for using asserts */
#include <stdio.h>  /* for printf */

#include "string.h" /* for using StrLen */

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
		write = last_char_saw + 1;
	}
	else
    {
        write = str;
    }
	
     *write = '\0';

    return str;
}

int IsPalindrome(const char* str)
{
	size_t left = 0, right = 0;
	
	assert(NULL != str);
	
	right = strlen(str);
	if (right <= 1)
	{
		return 1;
	}
	
	--right;
	for (; left <= right; ++left, --right)
	{
		if (str[left] != str[right])
		{
			return 0;
		}
	}
	
	return 1;
}
