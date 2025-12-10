
#include <assert.h>    /* for asserts using */
#include <stdio.h>     /* for using prints */
#include <string.h>    /* for strlen, strcmp, etc.. */
#include <stdlib.h>    /* for malloc alocate */
#include "arrays.h" /* for our functions */


typedef struct  
{
    size_t input_n;
    size_t expected_res; 
    const char* desc;    /* description what the test doing */
} test_case;

void Print_Test_Start(char* func_name);
void Print_Test_End(char* func_name);
void resetArray(int* arr, size_t size);
void printArray1(int* arr, size_t size);
void RunTest(int test_num, test_case tc);


void test_AllDataTypesPrint(void);
void test_Josephus(void); 



int main()
{
	test_AllDataTypesPrint();
	test_Josephus();
	
	return 0;
}

void RunTest(int test_num, test_case tc) 
{
    size_t result;
    
	printf("tc input is %lu\n", tc.input_n);
	result = Josephus(tc.input_n);

    if (result == tc.expected_res) 
    {
        printf("[PASS] Test %d (%s): Input %lu -> Output %lu\n", 
               test_num, tc.desc, (unsigned long)tc.input_n, (unsigned long)result);
    } 
    else
    {
        printf("[FAIL] Test %d (%s): Input %lu\n", 
               test_num, tc.desc, (unsigned long)tc.input_n);
        printf("Expected: %lu\n", (unsigned long)tc.expected_res);
        printf("Got:      %lu\n", (unsigned long)result);
    }
}

void test_Josephus(void)
{ 
	
     test_case tests[] =
      {
        {1, 1, "One soldier (Base Case)\n"},
        
        {2, 1, "Two soldiers\n"},
        {3, 3, "Three soldiers\n"},
        
        {4, 1, "Four soldiers "},
        {5, 3, "Five soldiers"},
        {6, 5, "Six soldiers"},
        {7, 7, "Seven soldiers (Ends with last man)"},

        /* Power of 2 Checks (Survivor should always be 1) */
        {8, 1,  "Eight soldiers "},
        {16, 1, "Sixteen soldiers "},
        {32, 1, "Thirty-Two soldiers "},

       	
        {10, 5,  "Ten soldiers"},
        {41, 19, "Forty-One soldiers"}
        
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);
    int i;

    printf("Running %d tests for Josephus Function...\n", num_tests);
    printf("---------------------------------------------------\n");

    for (i = 0; i < num_tests; i++) 
    {
        RunTest(i + 1, tests[i]);
    }

    printf("---------------------------------------------------\n"); 
}


void test_AllDataTypesPrint(void)
{
	Print_Test_Start("AllDataTypesPrint");
	AllDataTypesPrint();
	Print_Test_End("AllDataTypesPrint");
}



void resetArray(int* arr, size_t size)
{
	size_t i;
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = 0;	
	}
}

void printArray1(int* arr, size_t size)
{
	size_t i;
	
	for(i = 0; i < size; ++i)
	{
		printf("%d, ", arr[i]);
	}
	
	printf("\n");
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
