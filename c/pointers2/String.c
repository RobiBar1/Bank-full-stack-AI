#include <stddef.h> /* size_t type */
#include <assert.h> /* for asserts use */
#include <stdlib.h> /* Required for malloc() and free() */

size_t StrLen(const char *str);
char* StrCpy(char* dst, char* src);

char* StrTok(char* str, const char* delim)
{
	static char* last_str = NULL;
	static size_t last_index;
	size_t i, deli_index, out_index = 0;
	int found_delimiter = 0;
	
	assert(NULL != delim || (NULL != str && NULL == last_str));
	
	if (NULL == last_str || str != last_str)
	{
		last_str = str;
		i = 0;
	}
	else
	{
		i = last_index;
	}
	if ('\0' != str[i])
	{
		return NULL;
	}
	
	out_index = i;	
	while ('\0' != str[i])
	{
		for (deli_index = 0; '\0' != delim[deli_index]; ++deli_index)
		{
			if (delim[deli_index] == str[i])
			{
				last_index = i + 1;
				found_delimiter++;
				break;
			}
		}
		
		if (found_delimiter)
		{
			break;
		}
		
		i++;
	}
	
	return &str[out_index];
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
	size_t stack_index, needle_index, stack_size;
	
	assert(NULL != haystack && NULL != needle);
	
	stack_size = StrLen(haystack);
	if ('\0' == needle[0])
	{
		return 	(char*)haystack;
	}
	
	for (stack_index = 0, needle_index = 0; stack_index < stack_size - 1; ++stack_size)
	{
		if(haystack[stack_index] == needle[needle_index])
		{
			needle_index++;
			if('\0' == needle[needle_index])
			{
				stack_index++;
				return (char*)&haystack[stack_index - needle_index];
			}
		}
		else
		{
			needle_index = 0;
		}
	}
	
	return NULL;
}

char* StrNCat(char* dst, const char* src, size_t n_bytes)
{
	char* out_arr;
	size_t i, j, out_arr_size, left_size, right_size;
	
	assert(NULL != dst && NULL != src );
	
	left_size = StrLen(dst);
	right_size = StrLen(src);
	out_arr_size =  left_size + right_size + 1;
	out_arr = (char*)malloc(sizeof(char) * out_arr_size);
	
	for (i = 0; i < left_size && i < n_bytes; ++i)
	{
		out_arr[i] = dst[i];
	}
	
	for (j = 0; i < right_size && i < n_bytes; ++j, ++i)
	{
		out_arr[i] = src[j];
	}
	
	while('\0' != out_arr[i])
	{
		out_arr[i] = '\0';		
	}
	
	return out_arr;
}

char* StrCat(char* dst, const char* src)
{
	char* out_arr;
	size_t i, j, out_arr_size, left_size, right_size;
	
	assert(NULL != dst && NULL != src );
	
	left_size = StrLen(dst);
	right_size = StrLen(src);
	out_arr_size =  left_size + right_size + 1;
	out_arr = (char*)malloc(sizeof(char) * out_arr_size);
	
	for (i = 0; i < left_size; ++i)
	{
		out_arr[i] = dst[i];
	}
	
	for (j = 0; i < right_size; ++j, ++i)
	{
		out_arr[i] = src[j];
	}
	
	out_arr[i] = '\0';
	
	return out_arr;
}

char* StrDup(const char* str)
{
	char* out_arr;
	
	assert(NULL != str );
	
	out_arr = (char*)malloc(sizeof(char) * (StrLen(str) + 1));
	if (NULL == out_arr)
	{
		return NULL;
	}
	
	return StrCpy(out_arr, (char*)str);
}

char makeLower(char ch)
{
	return ch + 32;
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
	
	assert(NULL != str && c > 0);
	
	for (i = 0; '\0' != str[i]; ++i)
	{
		if (str[i] == c)
		{
			return (char*)&str[i];
		}
	}
	
	return (char*)&str[i];
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
		if(s1[i] >= 'A' && s1[i] <= 'Z')
		{
			left = makeLower(s1[i]);
		}
		else
		{
			left = s1[i];
		}
		if(s2[i] >= 'A' && s2[i] <= 'Z')
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


