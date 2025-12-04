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
		
		if (0 > num)
		{
			number_to_return = 1 / number_to_return;
		}
	}
	
	return number_to_return;
}


