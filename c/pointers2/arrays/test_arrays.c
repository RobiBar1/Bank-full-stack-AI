
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
void test_TwoDArraysBestPractice(void);


int main()
{
	test_AllDataTypesPrint();
	test_Josephus();
	test_TwoDArraysBestPractice();
	return 0;
}

int** create_matrix(size_t rows, size_t cols) 
{
    size_t i, j, numbers = 1;
    int** matrix = (int**)malloc(rows * sizeof(int*));
    
	if (NULL == matrix)	
	{
		return NULL;
	}
	
    for (i = 0; i < rows; i++) 
    {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (NULL == matrix[i])	
		{
			return NULL;
		}
		
        for (j = 0; j < cols; j++) 
        {
            matrix[i][j] = numbers;
            numbers++;
        }
    }
    
    return matrix;
}

void free_matrix(int** m, size_t rows) 
{
    size_t i;
    
    assert(NULL != m);
    
    for (i = 0; i < rows; i++) 
    {
        free(m[i]);
        m[i] = NULL;
    }
    
    free(m);
    m = NULL;
}

void test_TwoDArraysBestPractice(void)
{
    int** m = create_matrix(3, 3);
    
   	assert(NULL != m);
   	
	Print_Test_Start("TwoDArraysBestPractice");
    /* Expected: Row 1 = 6, Row 2 = 15, Row 3 = 24 */
	printf("[TEST] Standard 3x3 Matrix\n");
	printf("Expected: Row 1 = 6, Row 2 = 15, Row 3 = 24\n");
	    
    TwoDArraysBestPractice(m, 3, 3);
    
    free_matrix(m, 3);
    
    m = create_matrix(2, 4);
    
    assert(NULL != m);
    
    /* Expected: Row 1 = 10, Row 2 = 26 */
    printf("[TEST] Standard 3x3 Matrix\n");
    printf("Expected: Row 1 = 10, Row 2 = 26\n");
    TwoDArraysBestPractice(m, 2, 4);
    free_matrix(m, 2);
    Print_Test_End("TwoDArraysBestPractice");
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
