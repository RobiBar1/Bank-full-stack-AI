#include <stdio.h> /* For use printf */

void PrintHelloWorld(void)
{
	char* string = "\"hello world\"";
	int i;
	
	for(i = 0; i < 13; ++i )
	{
		printf("decimal = '%c' and in hexdecimal = '%x'\n", string[i], string[i]);
	}
}


float Power10(int num)
{
	float number_to_return = 1;
	int is_negetive = 0;
	
	if(0 > num)
	{
		is_negetive++;
		num *= -1;
	}
	
	if (0 == num)
	{
		
	}
	else
	{
		while(num > 0)
		{
			number_to_return *= 10;
			num--;
		}
		
		if (is_negetive)
		{
			number_to_return = 1 / number_to_return;
		}
	}
	
	return number_to_return;
}

int ReverseNumber(int num)
{
	int is_negetive = 0, rev_num = 0, right_num = 0;
	
	if (0 == num)
	{
		return 0;
	}
	if (num < 0)
	{
		num *= -1;
		is_negetive = 1;
	}
	
	rev_num = num % 10;
	num /= 10;
	while (num > 0)
	{
		rev_num *= 10;
		right_num = num % 10;
		rev_num += right_num;
		num /= 10;
	}
	
	
	if(is_negetive)
	{
		rev_num *= -1;
	}
	
	return rev_num;	
}

void Swap(int* one, int* other)
{
	int tmp;
	
	if (NULL == one || NULL == other)
	{
		return;
	}
	
	tmp = *one;
	*one = *other;		
	*other = tmp;
}
