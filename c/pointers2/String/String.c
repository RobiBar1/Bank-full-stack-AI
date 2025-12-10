#include <stddef.h> /* size_t type */
#include <assert.h> /* for asserts use */
#include <stdlib.h> /* for malloc and free */

size_t StrLen(const char *str);
char* StrCpy(char* dst, char* src);

char* StrTok(char* str, const char* delim)
{
	static char* next_token = NULL;
    char* current_token = NULL;
    int is_delim;
    size_t i;

    if (NULL != str)
    {
        next_token = str;
    }
    
    if (NULL == next_token)
    {
        return NULL;
    }

    while ('\0' != *next_token)
    {
        is_delim = 0;
        for (i = 0; '\0' != delim[i]; ++i)
        {
            if (*next_token == delim[i]) 
            {
                is_delim = 1;
                break;
            }
        }
        
        if (!is_delim)
        { 
            break; 
        }
            
        next_token++;
    }

    if ('\0' == *next_token)
    {
        next_token = NULL;
        return NULL;
    }

    current_token = next_token;

    while ('\0' != *next_token)
    {
        is_delim = 0;
        for (i = 0; '\0' != delim[i]; ++i)
        {
            if (*next_token == delim[i])
            {
                is_delim = 1;
                break;
            }
        }

        if (is_delim)
        {
            *next_token = '\0';
            next_token++;
            
            return current_token;
        }

        next_token++;
    }

    next_token = NULL;
    
    return current_token;
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
	size_t i, j;

    assert(NULL != haystack && NULL != needle);

    if ('\0' == needle[0])
    {
        return (char*)haystack;
    }

    for (i = 0; '\0' != haystack[i]; ++i)
    {
        for (j = 0; '\0' != needle[j]; ++j)
        {
            if (haystack[i + j] != needle[j])
            {
                break;
            }
        }

        if ('\0' == needle[j])
        {
            return (char*)&haystack[i];
        }
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
	
	assert(NULL != str );
	
	out_arr = (char*)malloc(sizeof(char) * (StrLen(str) + 1));
	if (NULL == out_arr)
	{
		return NULL;
	}
	
	return StrCpy(out_arr, (char*)str);
}

char MakeLower(char ch)
{
	return ch + ('a' - 'A');
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
	size_t i;
	char ch = (char)c;
	
	assert(NULL != str);
	
	if ('\0' == ch)
    {
        return (char*)str;
    }
    
	for (i = 0; '\0' != str[i]; ++i)
	{
		if (str[i] == ch)
		{
			return (char*)&str[i];
		}
	}
	
	return NULL;
}

int StrNCmp(const char* s1, const char* s2, size_t n)
{
	size_t i;
	int curent_res;
	
	assert(NULL != s1 && NULL != s2);
	
	for (i = 0; '\0' != s1[i] && '\0' != s2[i] && i < n; ++i)
	{
		curent_res = CharCmp(s1[i], s2[i]);
		if (0 == curent_res)
		{
			continue;
		}
		
		return curent_res;
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
		if('A' <= s1[i] && 'Z' >= s1[i])
		{
			left = makeLower(s1[i]);
		}
		else
		{
			left = s1[i];
		}
		if('A' <= s2[i] && 'Z' >= s2[i])
		{
			right = makeLower(s2[i]);
		}
		else
		{
			right = s2[i];
		}
		
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
	size_t i;
	
	assert(NULL != dst && NULL != src);
	
	for (i = 0; i < n_bytes && '\0' != src[i] && '\0' != dst[i]; i++)
	{
		dst[i] = src[i];
	}
	
	while ('\0' != dst[i])
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
	
	for (i = 0; '\0' != src[i]; i++)
	{
		dst[i] = src[i];
	}
	
	dst[i] = '\0';
	
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


