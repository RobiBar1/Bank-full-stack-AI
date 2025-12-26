#include <stdlib.h> /* free */
#include <assert.h> /* assert */
#include <stdio.h> /* printf */
#include "String.h" /* Strdup, StrLen in main test */

#define INIT0 0 										/* evrey where */
#define IS_NEGETIVE ('-' == (**c)) 						/* IsNegetive */
#define NEGETIVE -1				  						/* IsNegetive */		
#define	NORMAL_NUMBER 0			 						/* IsNegetive */
#define INT_VAL_OF_CHAR ((*str) - '0')  				/* Atoi10 */
#define GIVE_RIGHT_NUMBER_AS_NUM ((rev_num) % 10 + '0') /* Itoa10 */
#define BASE(X) X 				       					/* Itoa10, Atoi10 */
#define IS_NUMBER(c) ((c) >= '0' && (c) <= '9') 			/* Atoi10 */
#define VAL_IS_NEGETIVE ((value) < 0) 					/* Itoa10 */
#define EDGE_CASE_EQUAL0(x) (0 == (x))
#define L_VAL_IS_POSITIVE ((l_val) > 0)
static int IsNegetive(const char** c)
{
	if (IS_NEGETIVE)
	{
		++(*c);
		return NEGETIVE;
	}
	
	return NORMAL_NUMBER;
}

void WhiteSpace(const char* str)
{
	while (' ' == *str || '\t' == *str)
	{
		++str;
	}
}

static int Atoi10(const char* str)
{
	int num = INIT0;
	short is_negetive = INIT0;
	
	assert(NULL != str);
	
	WhiteSpace(str);
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

int Atoi(const char* str, char base)
{
	
}

char* Itoa10(int value, char* str)
{
	long rev_num = INIT0, l_val = (long)value;
	short is_negetive = INIT0, count = INIT0,
		  i = INIT0; 
	
	assert(NULL != str);
	
	if (VAL_IS_NEGETIVE)
	{
		l_val = -l_val;		
		is_negetive = NEGETIVE;
	}
	
	if (EDGE_CASE_EQUAL0(value))
	{
		return 0;
	}
	
	while (L_VAL_IS_POSITIVE)
	{
		rev_num *= BASE(10);
		rev_num += l_val % BASE(10);
		l_val /= BASE(10);
	}
	
	if (is_negetive == NEGETIVE)
	{
		str[i++] = '-';
		++count;
	}
	
	for (; i < count; ++i)
	{
		str[i] = GIVE_RIGHT_NUMBER_AS_NUM;
		rev_num /= BASE(10);
	}
	
	return str;
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
	
	while (left <= right)
	{
		SwapChar(&(str[left++]), &(str[right--]));
	}
	
}

char* Itoa(int value, char* str, unsigned char base)
{
	int i = INIT0, is_negetive = INIT0;
	long l_val = (long) value;
	char jump_table[] = 
	{
	'0','1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};					
	
	assert(NULL != str);
	
	if (EDGE_CASE_EQUAL0(value))
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	
	if (value < 0)
	{
		if (10 != base)
		{
			str[i++] = '0';
			str[i] = '\0';
			return str;
		}
		
		str[i++] = '-';
		++is_negetive;
		l_val = -l_val;
	}
	
	while (l_val > 0)
	{
		str[i++] = jump_table[l_val % base];
		l_val /= base;
	}
	
	str[i] = '\0';
	ReverseStringInPlace(str + is_negetive, i - is_negetive);
	
	return str;
}

int main()
{	
	char itoa_out[32] = {"heylo"};
	
	Itoa(0, itoa_out, (unsigned char)8);
	printf("itoa_out is: %s\n", itoa_out);

	Itoa(-120, itoa_out, (unsigned char)10);
	printf("itoa_out is: %s\n", itoa_out);
	
	Itoa(120, itoa_out, (unsigned char)16);
	printf("itoa_out is: %s\n", itoa_out);
	
	Itoa(120, itoa_out, (unsigned char)2);
	printf("itoa_out is: %s\n", itoa_out);
	return 0;
}
