#include <assert.h> /* for using asserts */
#include "String.h" /* for using StrLen */

int ISPalindrome(const char* str)
{
	size_t i, j;
	
	assert(str != NULL);
	
	j = StrLen(str);
	j--;
	if (j < 1)
	{
		return 1;
	}
	
	for (i = 0; i <= j; i++, j--)
	{
		if (str[i] != str[j])
		{
			return 0;
		}
	}
	
	return 1;
}
