#include <stddef.h> /* size_t type */
#include <assert.h> /* for asserts use */
#include <stdlib.h> /* for malloc and free */

size_t StrLen(const char *str);
char* StrCpy(char* dst, char* src);
char* StrChr(const char* str, int c);
char* StrStr(const char* haystack, const char* needle);
int StrNCmp(const char* s1, const char* s2, size_t n);

char* StrTok(char* str, const char* delim)
{
	static char *remind_token;
	char* current_token = NULL;
	char* start = NULL;
	
	if (NULL == str)
	{
		if (NULL == remind_token)
		{
			return NULL;
		}
		
		current_token = remind_token;
	}
	else
	{
		current_token = str;
	}
	
	/* for skip prefix delimeter, exmpl: ,,,,,hello -> will jump to h */
	while (*current_token && StrChr(delim, *current_token))
	{
		++current_token;
	}
	
	/* exmple: ,,,,,,,,,,,,,'\0' */
	if (!*current_token)
	{
		remind_token = NULL;
		return NULL;
	}
	
	start = current_token;
	
	/* end when (start to current) is the next word */
	while (*current_token && !StrChr(delim, *current_token))
	{
		++current_token;
	}
	
	if (*current_token)
	{
		*current_token = '\0';
		remind_token = current_token + 1;
	}
	else
	{
		remind_token = NULL;
	}
	
	return start;
}

size_t StrSpn(const char* str, const char* accept)
{
	size_t str_i, accept_i, out_count_init_equals_chars = 0, is_init = 0;
	
	for (str_i = 0; '\0' != str[str_i]; ++str_i)
	{
		for (accept_i = 0; '\0' != accept[accept_i]; ++accept_i)
		{
			if (str[str_i] == accept[accept_i])
			{
				out_count_init_equals_chars++;
				is_init++;
				break;	
			}
		}
		
		if (is_init)
		{
			is_init = 0;
			continue;
		}
		
		break;
	}
	
	return out_count_init_equals_chars;
}

char* StrStr(const char* haystack, const char* needle)
{
	size_t needle_size = 0;

    assert(NULL != haystack);
    assert(NULL != needle);

    if (!*needle)
    {
        return (char*)haystack;
    }
    
    needle_size = StrLen(needle);
    while(*haystack)
    {
		if (!StrNCmp(haystack, needle, needle_size))
		{
			return (char*)haystack;
		}

		++haystack;
	}
	
    return NULL;
}

char* StrNCat(char* dst, const char* src, size_t n_bytes)
{
	char* original_dst = NULL;
    size_t i;

    assert(NULL != dst && NULL != src);
	
	original_dst = dst;
    while ('\0' != *dst)
    {
        dst++;
    }

    for (i = 0; i < n_bytes && '\0' != src[i]; i++)
    {
        *dst = src[i];
        dst++;
    }

    *dst = '\0';

    return original_dst;
}

char* StrCat(char* dst, const char* src)
{
	char* out_original_dst = NULL;

    assert(NULL != dst && NULL != src);
	
	out_original_dst = dst;
    while ('\0' != *dst)
    {
        dst++;
    }

    while ('\0' != *src)
    {
        *dst = *src;
        dst++;
        src++;
    }

    *dst = '\0';

    return out_original_dst;
}

char* StrDup(const char* str)
{
	char* out_arr = NULL;
	
	assert(NULL != str);
	
	out_arr = (char*)malloc(sizeof(char) * (StrLen(str) + 1));
	if (NULL == out_arr)
	{
		return NULL;
	}
	
	return StrCpy(out_arr, (char*)str);
}

char MakeLower(char ch)
{	
	if('A' <= ch && 'Z' >= ch)
	{
		return ch + ('a' - 'A');	
	}
	
	return ch;
}

int CharCmp(char ch, char ch1)
{
	if (ch == ch1)
	{
		return 0;
	}
	if (ch < ch1)
	{
		return -1;
	}
	
	return 1;
}

char* StrChr(const char* str, int c)
{	
	assert(NULL != str);
    
	while (*str)
	{
		if (*str++ == c)
		{
			--str;
			
			return (char*)str;
		}
	}
	
	if (!c)
	{
		return (char*)str;
	}
	
	return NULL;
}

int StrNCmp(const char* s1, const char* s2, size_t n)
{
    size_t i = 0;
    int current_res = 0;
    
    assert(NULL != s1);
    assert(NULL != s2);
    
    for (; i < n; ++i)
    {
        current_res = CharCmp(s1[i], s2[i]);
        if (0 != current_res)
        {
            return current_res;
        }
        
        if (!s1[i])
        {
            return 0;
        }
    }
    
    return 0;
}

int StrCaseCmp(const char* s1, const char* s2)
{
	size_t i;
	int curent_res;
	char left, right;
	
	assert(NULL != s1 && NULL != s2);
	
	for (i = 0; '\0' != s1[i] && '\0' != s2[i]; ++i)
	{
		
		left = MakeLower(s1[i]);
		right = MakeLower(s2[i]);
		
		curent_res = CharCmp(left, right);
		if (0 == curent_res)
		{
			continue;
		}
		
		return curent_res;
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


char* StrNCpy(char* dst, const char* src, size_t n_bytes)
{
	char* start = dst;
	
	assert(NULL != dst);
	assert(NULL != src);
	
	for (; n_bytes > 0 && *src; --n_bytes)
	{
		*dst++ = *src++;
	}
	
	while ( n_bytes > 0)
	{
		*dst++ = '\0';
		--n_bytes;
	}
	
	return start;
}

/*
 * that was the original h.file, and then few change it to:
 * char* StrCpy(char* dst, const char* src) */
char* StrCpy(char* dst, char* src) 
{
	char* start_index = dst;
	
	assert(NULL != dst);
	assert(NULL != src);

	while ((*dst++ = *src++));
	
	return 	start_index;
}


size_t StrLen(const char *str)
{
	size_t i = 0;
	
	assert(NULL != str);
	
	while (('\0' != str[i++]));
		
	return i - 1;
}

int StrCmp(const char *s1, const char *s2)
{
	size_t i;
	int curent_res;
	
	assert(NULL != s1 && NULL != s2);
	
	for (i = 0; '\0' != s1[i] && '\0' != s2[i]; ++i)
	{
		curent_res = CharCmp(s1[i], s2[i]);
		if (0 == curent_res)
		{
			continue;
		}
		
		return curent_res;
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


