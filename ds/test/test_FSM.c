#include <stdio.h>

#include "FSM.c"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"


static void PrintStart(char* name_function)
{
	printf("================================================================\n");
	printf("------------------- Start Test: %s ------------------------\n", name_function);
}
static void PrintEnd(char* name_function)
{
	printf("------------------- End Test: %s --------------------------\n", name_function);
	printf("================================================================\n");
}

static void TestIsValidString(void)
{
	char* test[] = {"011010", "0", "01", "1", "10", "011101", "00", "000000000", "00nooneher00", "111111111", "1111000"};
  	int expeted_result[] = {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
  	int real_result = 0;
  	int total_tests = 0;
  	int total_tests_pass = 0;
	size_t i = 0;
	
	PrintStart("IsValidString");
	for (; i < 11; ++i)
	{
		real_result = IsValidString(test[i]);
		++total_tests;
		printf("input \"%s\" is: %s ", test[i], real_result ? GREEN "VALID " RESET : RED "NOT VALID " RESET);
		if (real_result == expeted_result[i])
		{
			printf(GREEN "Correct Answer!\n" RESET);
			++total_tests_pass;
		}
		else
		{
			printf(RED "Wrong Answer!\n" RESET);
		}
		
	}
	
	PrintEnd("IsValidString");
	
	total_tests == total_tests_pass ? printf(GREEN) : printf(RED);
	printf("you pass: %d/%d tests.\n" RESET, total_tests_pass, total_tests);	
}

int main() 
{
	TestIsValidString();
	
	return 0;
}
