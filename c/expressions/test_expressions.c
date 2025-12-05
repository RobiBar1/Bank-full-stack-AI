#include <stdio.h> /* For print */
#include <math.h> /* Required for fabs() */

#include "expressions.h" /* For implement of functions */

#define EPSILON 0.00001f /* The allowed margin of error */

void Test_Power10();
void Test_PrintHelloWorld();
void Test_ReverseNumber();


int main() 
{
	Test_PrintHelloWorld();
	Test_Power10();
	Test_ReverseNumber();
	return 0;
}

void Print_Test_Start(char* func_name)
{
	printf("--------------------------------------------------------------\n");
	printf("start testing for: \"%s()\" \n", func_name);	
}
void Print_Test_End(char* func_name)
{
	printf("Done SECCSFUL testing for: \"%s()\" \n", func_name);
	printf("--------------------------------------------------------------\n");	
}

void Test_PrintHelloWorld()
{
	Print_Test_Start("PrintHelloWorld");
	PrintHelloWorld();	
	Print_Test_End("PrintHelloWorld");
}

void Test_Power10()
{
	float exepted = 1000, result = Power10(3);
	
	Print_Test_Start("Power10");
	if(fabs(exepted - result) > EPSILON)
	{
		printf("Power10 Fail with input 3\n");
		return;
	}
	
	exepted = 1, result = Power10(0);	
	if(fabs(exepted - Power10(0)) > EPSILON)
	{
		printf("Power10 Fail with input 0\n");
		return;
	}
	
	exepted = 0.01, result = Power10(-2);
	if(fabs(exepted - result) > EPSILON)
	{
		printf("Power10 Fail with input -2\n");
		return;
	}
	
	Print_Test_End("Power10");
}

void Test_ReverseNumber()
{
	int exepted, result;
	
	Print_Test_Start("ReverseNumber");
	exepted = 4321, result = ReverseNumber(1234);
	if (exepted != result)
	{
		printf("ReverseNumber Fail with input 1234\n");
	}
	
	exepted = 5, result = ReverseNumber(5);
	if (exepted != result)
	{
		printf("ReverseNumber Fail with input 5\n");
	}
	
	exepted = 4321, result = ReverseNumber(1234);
	if (exepted != result)
	{
		printf("ReverseNumber Fail with input 1234\n");
	}
	
	exepted = 4321, result = ReverseNumber(12340);
	if (exepted != result)
	{
		printf("ReverseNumber Fail with input 12340\n");
	}
	
	exepted = -51, result = ReverseNumber(-15);
	if (exepted != result)
	{
		printf("ReverseNumber Fail with input 12340\n");
	}
	
	exepted = -51, result = ReverseNumber(-15);
	if (exepted != result)
	{
		printf("ReverseNumber Fail with input 12340\n");
	}
	
	Print_Test_End("ReverseNumber");
}

