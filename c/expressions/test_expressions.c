#include <stdio.h> /* For print */
#include "expressions.h" /* For implement of functions */

void Test_Power10();
void Test_PrintHelloWorld();

int main() 
{
	Test_PrintHelloWorld();
	Test_Power10();
	return 0;
}

void Test_PrintHelloWorld()
{
	printf("start PrintHelloWorld function\n");
	PrintHelloWorld();	
	printf("end SECCSFUL PrintHelloWorld function\n");
}

void Test_Power10()
{
	printf("start Test_Power10 function\n");
	if(1000 != Power10(3))
	{
		printf("Power10 Fail with input 3");
		return;
	}
	else if(1 != Power10(0))
	{
		printf("Power10 Fail with input 0");
		return;
	}
	else if( (1/100) != Power10(-2))
	{
		printf("Power10 Fail with input -2");
		return;
	}
	
	printf("end SECCSFUL Test_Power10 function\n");
}
