#include <stdio.h>  /* for printf use */
#include <assert.h> /* for asserts use */
#include <stdlib.h> /* for mallocs use */

#include "String.h"

#define ROWS 3
#define COLS 4

/*--------------------- 2D Arrays diffrent declretions resarch ---------------



 options 1: Fixed-size array (columns must be known at compile time) 
Advantages:
 *   - Simple and intuitive syntax
 *   - Compiler knows the layout, enables arr[i][j] syntax
 *   - Type-safe: compiler checks array dimensions
 * 
 * Disadvantages:
 *   - Column size must be a compile-time constant
 *   - Not flexible for different sized arrays
 *   - ROWS is ignored (decays to pointer anyway)

void TwoDArraysNotDynamic(int arr[][COLS], int rows)
{
    printf("Method 1: Fixed-size array\n");
    printf("  Access arr[1][2] = %d\n", arr[1][2]);
}

//Alternative syntax - exactly equivalent to above 
void TwoDArraysNotDynamic1(int arr[ROWS][COLS])
{
    printf("Method 1 Alt: Explicit dimensions\n");
    printf("  Access arr[1][2] = %d\n", arr[1][2]);
}

-----------------------------------------------------------------------
option 2: Pointer to array (pointer to row)
 * Signature: int (*arr)[COLS]
 * 
 * Advantages:
 *   - Makes pointer nature explicit
 *   - Clearer that we receive a pointer, not actual array
 *   - Works with dynamically allocated rows of fixed column size
 * 
 * Disadvantages:
 *   - Syntax can be confusing (parentheses required)
 *   - Still requires fixed column size at compile time
 
void TwoDArraysHalfDynamic(int (*arr)[COLS], int rows)
{
    printf("Method 2: Pointer to array\n");
    printf("  Access arr[1][2] = %d\n", arr[1][2]);
}

-----------------------------------------------------------------------
 option 3: Pointer to pointer (double pointer)
 * Signature: int **arr
 * 
 * Advantages:
 *   - Fully flexible dimensions (runtime determined)
 *   - Works with jagged arrays (rows of different lengths)
 *   - Can resize dynamically
 * 
 * Disadvantages:
 *   - Requires special allocation (array of pointers + data)
 *   - NOT compatible with static 2D arrays!
 *   - Extra indirection (slightly slower)
 *   - More complex memory management
 *
 * THIS IS BEST PRACTICE AS I FIGER OUT(best mix between flex, efficient and readabilty).
 */
 
int TwoDArraysBestPractice(int **arr, size_t rows, size_t cols)
{
	size_t i = 0, j = 0;
	int *arr_res_by_rows = NULL;
	
	assert(NULL != arr);
	assert(NULL != *arr);
    
    printf("sizeof(arr) = %lu bytes\n", (unsigned long)sizeof(arr));
	arr_res_by_rows = (int*)calloc(rows, sizeof(int));
	if (NULL == arr_res_by_rows)
	{
		return 1;
	}
	
    for (; i < rows; ++i)
    {
        for (j = 0; j < cols; ++j)
        {
            arr_res_by_rows[i] += arr[i][j];
			printf("arr_res_by_rows[i] is: %d\n", arr[i][j]);
        }
    }
    
    for (i = 0; i < rows; ++i)
    {
    	printf("the sum or row %lu is: %d .\n", i + 1, arr_res_by_rows[i]);
    }
	
	free(arr_res_by_rows); arr_res_by_rows = NULL;
	
	return 0;
}

/*-----------------------------------------------------------------------
 * option: 4: Flattened 1D array with manual indexing
 * Signature: int *arr  (with rows and cols parameters)
 * 
 * Advantages:
 *   - Maximum flexibility
 *   - Works with any contiguous memory block
 *   - Compatible with static 2D arrays (cast needed)
 *   - Single memory block (cache friendly)
 * 
 * Disadvantages:
 *   - Manual index calculation: arr[i * cols + j]
 *   - Less readable access syntax
 *   - Easy to make indexing errors
 *   
 * THIS IS OFTEN THE BEST CHOICE FOR C89!
 
 
void TwoDArraysFullDynamicFullEfficient(int *arr, int rows, int cols)
{
    printf("Method 4: Flattened array\n");
    printf("  Access arr[1*cols+2] = %d\n", arr[1 * cols + 2]);
}

-----------------------------------------------------------------------
 option 5: Array of pointers:
 * Signature: int *arr[]  or  int *arr[ROWS]
 * 
 * Advantages:
 *   - Allows jagged arrays (different row lengths)
 *   - arr[i][j] syntax works
 *   - Rows can be allocated separately
 * 
 * Disadvantages:
 *   - Extra storage for pointer array
 *   - Non-contiguous memory possible
 *   - Not compatible with static 2D arrays directly
 
 
void TwoDArraysFullDynamic(int *arr[], int rows, int cols)
{
    printf("Method 5: Array of pointers\n");
    printf("  Access arr[1][2] = %d\n", arr[1][2]);
}


// ----------------------------------Josef Functions----------------------*/

char MakeLower(char ch);

void InitJosefCircle(size_t* arr, size_t amount_of_pepole)
{
	size_t i = 1;
	
	assert(NULL != arr);
	
	arr[0] = 0;
	for (; i < amount_of_pepole - 1; ++i)
	{
		arr[i] = i + 1;
	}
	
	arr[i] = 1;
}

void KillManAndSwapTarget(size_t* arr, size_t killer_index)
{
	size_t get_his_kill = 0;
	
	assert(NULL != arr);
	
	if (arr[arr[killer_index]] == arr[killer_index])
	{
		arr[arr[killer_index]] = 0;	
	}
	else
	{
		get_his_kill = arr[arr[killer_index]];
		arr[arr[killer_index]] = 0;
		arr[killer_index] = get_his_kill;
	}
}

size_t JosefGameLoop(size_t* arr, size_t amount_of_pepole)
{
	size_t i = 1, have_circle_winner = 0;
	
	assert(NULL != arr);
	
	while (i < amount_of_pepole && !have_circle_winner) 
	{
		if (0 != arr[i])
		{
			KillManAndSwapTarget(arr, i);
			if (0 == arr[i])
			{
				have_circle_winner = i;
				break; 
			}
			
			i = arr[i];
		}
	}
	
	free(arr);
	arr = NULL;
	
	return i;
}

size_t Josephus(size_t amount_of_pepole) /* first man do first kill */
{
	size_t *arr = NULL;
	
	if (amount_of_pepole <= 1)
	{
		return amount_of_pepole;
	}
	
	amount_of_pepole++;
	arr = (size_t*)malloc(sizeof(size_t) * amount_of_pepole);
	if (NULL == arr)
	{
		printf("alocate memory problem");
		return 0;
	}
	
	InitJosefCircle(arr, amount_of_pepole);
	
	return JosefGameLoop(arr, amount_of_pepole);
}
 /*----------------------------------Environment functions----------------------*/

size_t CountEnvVars(const char* envp[])
{
    size_t count = 0;
    
    assert(NULL != envp);
    
    while ((NULL != envp[count++]));
    
    return count - 1;
}

void FreeEnvArray(char** env_copy, size_t count)
{
    size_t i = 0;
    
	assert(NULL != env_copy);

    for (; i < count; i++)
    {
        if (NULL != env_copy[i])
        {
            free(env_copy[i]); env_copy[i] = NULL;
        }
    }
    
    free(env_copy); env_copy = NULL;
}

char* CreateLowerCaseString(const char* source)
{
    size_t len = 0, i = 0;
    char* new_str = NULL;
    
    assert(NULL != source);
    
    len = StrLen(source);
    new_str = (char*)malloc((len + 1) * sizeof(char));
    if (NULL == new_str)
    {
        return NULL;
    }

    for (; i < len; ++i)
    {
        new_str[i] = MakeLower(source[i]);
    }
    
    new_str[len] = '\0';

    return new_str;
}

int EnvironmentVariblesPrint(const char* envp[])
{
	char** env_copy = NULL;
    size_t num_vars = 0;
    size_t i = 0;

    assert(NULL != envp);
    
    num_vars = CountEnvVars(envp);
    env_copy = (char**)malloc((num_vars + 1) * sizeof(char*));
    if (NULL == env_copy)
    {
        return 1; /* env_copy allocation fail*/
    }

    for (; i <= num_vars; ++i)
    {
        env_copy[i] = NULL;
    }

    for (i = 0; i < num_vars; ++i)
    {
        env_copy[i] = CreateLowerCaseString(envp[i]);
        if (NULL == env_copy[i])
        {
            FreeEnvArray(env_copy, num_vars);
            return 2; /* string allocation failed */
        }
    }

    for (i = 0; i < num_vars; ++i)
    {
        printf("%s\n", env_copy[i]);
    }

    FreeEnvArray(env_copy, num_vars);

    return 0; 
}

 /*----------------------------------DataTypePrint----------------------*/
void AllDataTypesPrint(void)
{
    char char_;
    signed char signed_char;
    unsigned char unsigned_char;

    short short_;
    short int short_int;
    signed short signed_short;
    signed short int signed_short_int;
    unsigned short unsigned_short;
    unsigned short int unsigned_short_int;

    int int_;
    signed signed_;
    signed int signed_int;
    unsigned unsigned_;
    unsigned int unsigned_int;

    long long_;
    long int long_int;
    signed long signed_long;
    signed long int signed_long_int;
    unsigned long unsigned_long;
    unsigned long int unsigned_long_int; 

    /* (unsigned long) cast for maximum C89 compatibility */
    
    printf("--- CHAR ---\n");
    printf("char:                   %lu bytes\n", (unsigned long)sizeof(char_));
    printf("signed char:            %lu bytes\n", (unsigned long)sizeof(signed_char));
    printf("unsigned char:          %lu bytes\n", (unsigned long)sizeof(unsigned_char));
    printf("\n");

    printf("--- SHORT ---\n");
    printf("short:                  %lu bytes\n", (unsigned long)sizeof(short_));
    printf("short int:              %lu bytes\n", (unsigned long)sizeof(short_int));
    printf("signed short:           %lu bytes\n", (unsigned long)sizeof(signed_short));
    printf("signed short int:       %lu bytes\n", (unsigned long)sizeof(signed_short_int));
    printf("unsigned short:         %lu bytes\n", (unsigned long)sizeof(unsigned_short));
    printf("unsigned short int:     %lu bytes\n", (unsigned long)sizeof(unsigned_short_int));
    printf("\n");

    printf("--- INT ---\n");
    printf("int:                    %lu bytes\n", (unsigned long)sizeof(int_));
    printf("signed:                 %lu bytes\n", (unsigned long)sizeof(signed_));
    printf("signed int:             %lu bytes\n", (unsigned long)sizeof(signed_int));
    printf("unsigned:               %lu bytes\n", (unsigned long)sizeof(unsigned_));
    printf("unsigned int:           %lu bytes\n", (unsigned long)sizeof(unsigned_int));
    printf("\n");

    printf("--- LONG ---\n");
    printf("long:                   %lu bytes\n", (unsigned long)sizeof(long_));
    printf("long int:               %lu bytes\n", (unsigned long)sizeof(long_int));
    printf("signed long:            %lu bytes\n", (unsigned long)sizeof(signed_long));
    printf("signed long int:        %lu bytes\n", (unsigned long)sizeof(signed_long_int));
    printf("unsigned long:          %lu bytes\n", (unsigned long)sizeof(unsigned_long));
    printf("unsigned long int:      %lu bytes\n", (unsigned long)sizeof(unsigned_long_int));
    printf("\n");
}
