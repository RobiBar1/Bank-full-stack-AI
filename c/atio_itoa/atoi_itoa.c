#include <assert.h> /* assert */
#include <stddef.h> /* NULL */
#include <stdio.h>  /* printf */

#define INIT0 0
#define IS_NEGETIVE ('-' == (**c))
#define NEGETIVE -1
#define NORMAL_NUMBER 0
#define INT_VAL_OF_CHAR ((*str) - '0')
#define BASE(X) X
#define IS_NUMBER(c) ((c) >= '0' && (c) <= '9')
#define VAL_IS_NEGETIVE ((value) < 0)
#define EDGE_CASE_EQUAL0(x) (0 == (x))
#define IS_LOWER_LETTER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_UPPER_LETTER(c) ((c) >= 'A' && (c) <= 'Z')
#define LOWER_TO_DIGIT(c) ((c) - 'a' + 10)
#define UPPER_TO_DIGIT(c) ((c) - 'A' + 10)
#define INVALID_DIGIT -1
#define IS_VALID_FOR_BASE(digit, base) ((digit) >= 0 && (digit) < (base))
#define CHAR_RANGE 256
#define NOT_SEEN 0
#define IN_ARR3 1
#define IN_ARR1 2
#define PRINTED 3
#define IS_LITTLE_ENDIAN (1 == *(unsigned char*)&1)

void PrintIntersectionFirstTwoWithExclusionThird(const char* arr_use_1, size_t size1, 
                                                  const char* arr_use_2, size_t size2, 
                                                  const char* arr_exclude_3, size_t size3)
{
    unsigned char lookup[CHAR_RANGE] = {NOT_SEEN};
    size_t i = INIT0;
    unsigned char c = INIT0;
    
    assert(NULL != arr_use_1);
    assert(NULL != arr_use_2);
    assert(NULL != arr_exclude_3);
    
    for (; i < size3; ++i)
    {
        c = (unsigned char)arr_exclude_3[i];
        lookup[c] = IN_ARR3;
    }
    
    for (i = INIT0; i < size1; ++i)
    {
        c = (unsigned char)arr_use_1[i];
        if (NOT_SEEN == lookup[c])
        {
            lookup[c] = IN_ARR1;
        }
    }
    
    for (i = INIT0; i < size2; ++i)
    {
        c = (unsigned char)arr_use_2[i];
        if (IN_ARR1 == lookup[c])
        {
            printf("%c", c);
            lookup[c] = PRINTED;
        }
    }
    
    printf("\n");
}

int IsLittleEndian(void)
{
    unsigned int x = 1;
    unsigned char* byte_ptr = (unsigned char*)&x;
    
    if (1 == *byte_ptr)
    {
        return 1;
    }
    
    return 0;
}

static int IsNegetive(const char** c)
{
	if (IS_NEGETIVE)
	{
		++(*c);
		return NEGETIVE;
	}
	
	return NORMAL_NUMBER;
}

void WhiteSpace(const char** str)
{	
	while (' ' == **str || '\t' == **str)
	{
		++(*str);
	}
}

static int CharToDigit(char c)
{
	if (IS_NUMBER(c))
	{
		return c - '0';
	}
	
	if (IS_LOWER_LETTER(c))
	{
		return LOWER_TO_DIGIT(c);
	}
	
	if (IS_UPPER_LETTER(c))
	{
		return UPPER_TO_DIGIT(c);
	}
	
	return INVALID_DIGIT;
}

static void SwapChar(char* c, char* c1)
{
	char tmp = 'u';
	
	assert(NULL != c);
	assert(NULL != c1);
	
	tmp = *c;
	*c = *c1;
	*c1 = tmp;
}

static void ReverseStringInPlace(char* str, int size)
{
	int right = size - 1, left = INIT0;
	
	assert(NULL != str);
	
	while (left < right)
	{
		SwapChar(&(str[left++]), &(str[right--]));
	}
}

int Atoi10(const char* str)
{
	int num = INIT0;
	short is_negetive = INIT0;
	
	assert(NULL != str);
	
	WhiteSpace(&str);
	is_negetive = IsNegetive(&str);
	
	while (IS_NUMBER(*str))
	{
		num *= BASE(10);
		num += INT_VAL_OF_CHAR;
		++str;
	}
	
	if (is_negetive == NEGETIVE)
	{
		num *= -1;
	}
	
	return num;
}

int Atoi(const char* str, unsigned char base)
{
	int num = INIT0;
	int digit_val = INIT0;
	short is_negetive = INIT0;
	short is_non_valid = INIT0;
	
	assert(NULL != str);
	
	WhiteSpace(&str);
	is_negetive = IsNegetive(&str);
	
	while ('\0' != *str && !is_non_valid)
	{
		digit_val = CharToDigit(*str);
		
		if (!IS_VALID_FOR_BASE(digit_val, base))
		{
			++is_non_valid;
		}
		else
		{
			num *= base;
			num += digit_val;
			++str;
		}
	}
	
	if (NEGETIVE == is_negetive)
	{
		num = -num;
	}
	
	return num;
}

char* Itoa10(int value, char* str)
{
	int i = INIT0;
	int is_negetive = INIT0;
	unsigned long ul_val = 0;
	
	assert(NULL != str);
	
	if (EDGE_CASE_EQUAL0(value))
	{
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	
	if (VAL_IS_NEGETIVE)
	{
		is_negetive = 1;
		ul_val = (unsigned long)(-(value + 1)) + 1;
	}
	else
	{
		ul_val = (unsigned long)value;
	}
	
	while (ul_val > 0)
	{
		str[i++] = (ul_val % BASE(10)) + '0';
		ul_val /= BASE(10);
	}
	
	if (is_negetive)
	{
		str[i++] = '-';
	}
	
	str[i] = '\0';
	ReverseStringInPlace(str, i);
	
	return str;
}

char* Itoa(int value, char* str, unsigned char base)
{
	int i = INIT0;
	int is_negetive = INIT0;
	unsigned long ul_val = 0;
	char jump_table[] = 
	{
		'0','1', '2', '3', '4', '5', '6', '7', '8', '9', 
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
		'u', 'v', 'w', 'x', 'y', 'z'
	};					
	
	assert(NULL != str);
	
	if (EDGE_CASE_EQUAL0(value))
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	
	if (value < 0)
	{
		is_negetive = 1;
		ul_val = (unsigned long)(-(value + 1)) + 1;
	}
	else
	{
		ul_val = (unsigned long)value;
	}
	
	while (ul_val > 0)
	{
		str[i++] = jump_table[ul_val % base];
		ul_val /= base;
	}
	
	if (is_negetive)
	{
		str[i++] = '-';
	}
	
	str[i] = '\0';
	ReverseStringInPlace(str, i);
	
	return str;
}
