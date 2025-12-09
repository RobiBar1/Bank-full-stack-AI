#include <assert.h> /* for using asserts */
#include "String.h" /* for using StrLen */


char* RemoveWhiteSpaces(char* str)
{
    size_t i, j, size, row_spaces = 0, real_chars = 0, is_start_mid = 0, start_spaces = 0, end_spaces = 0, last_index_saw_real_char = 0;
    char* out_arr;
 
	assert(NULL != str);
/* this for will map the spaces start+mid+end and count chars*/
	for (i = 0; '\0' != str[i]; ++i) 
	{
        if (9 == str[i] || 32 == str[i])
        {
               row_spaces++;
               switch (is_start_mid)
               {
                    case 0:
                        start_spaces++;
                        continue;
                    case 1:
                        if ( 1 == row_spaces)
                        {
                            if('\0' != str[i + 1])
                            {
                                if(9 != str[i + 1] && 32 != str[i + 1])
                                {
                                    real_chars++;    
                                }   
                                else
                                {
                                    row_spaces++
                                    i++;
                                }
                            }
                        }
                        
		continue;
               }
        }
        else
        {
            real_chars++;
            row_spaces = 0;
            last_index_saw_real_char = i;
            if (1 == real_chars)
            {
                   is_start_mid++;
            }
        }
	}
	/*   as  dda   \0
	   01234567891011   */
	end_spaces = i - last_index_saw_real_char - 1;
	out_arr = (char*)malloc(sizeof*(char) * (real_chars + 1));
	assert(NULL != out_arr);
 
	row_spaces = 0;
	/* this for will build the new string as need */
	for (i = start_spaces, j = 0; '\0' != str[i]; ++i)
	{
	    if (9 == str[i] || 32 == str[i])
	    {
            row_spaces++;
            if (1 == row_spaces)
            {
                if('\0' != str[i + 1])
                {
                    if(9 != str[i + 1] && 32 != str[i + 1])
                    {
                        out_arr[j] = str[i];
                        j++;
                        real_chars--;
                        if (!real_chars)
                        {
                            break;
                        }
 
                        continue;
                    }
                    else
                    {
                        row_spaces++
                        i++;
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
	    }
	    else
	    {
	        out_arr[j] = str[i];
            j++;
            real_chars--;
            if (!real_chars)
            {
                break;
            }
 
            continue;
	    }
	}
 
	return out_arr;
}
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
