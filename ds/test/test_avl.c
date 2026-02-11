/* gd src/avl.c test/test_avl.c -I include/ */

#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

#define TEST(name, real, expected) \
    do { \
        ++tests_run; \
        printf("%s: ", name); \
        if((size_t)(real) == (size_t)(expected)) { \
            ++tests_passed; \
            printf(GREEN "PASS" RESET "\n"); \
        } else { \
            printf(RED "FAIL" RESET "\n"); \
            printf(RED "      Expected: %ld, Got: %ld" RESET "\n", (size_t)(expected), (size_t)(real)); \
        } \
    } while(0)

static size_t tests_run = 0;
static size_t tests_passed = 0;

static void PrintStart(char* name_function)
{
	printf("================================================================\n");
	printf("------------------- Start Test: %s ------------------------\n", name_function);
}
static void PrintEnd(char* name_function)
{
	printf("================================================================\n");
	printf("------------------- End Test: %s --------------------------\n", name_function);
}


static int CmpFunc(const void* left, const void* right)
{
	return *(int*)left - *(int*)right;
}


static void TestAVLInsert(void)
{
	avl_t* avl = AVLCreate(CmpFunc);
	int arr[] = {10, 50, 30, 0, 15, 75, 100};
	int sorted_arr[] = {0, 10, 15, 30, 50, 75, 100};
	int arr_size = sizeof(arr) / sizeof(int);
	int i = 0;
	int is_good = 1;
	size_t height = 0;
	
	TEST("Test Height empty tree", AVLHeight(avl), 0);
	
	for (i = 0; i < arr_size; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}
	
	TEST("Test Height full tree", AVLHeight(avl), 4);
	if (4 == AVLHeight(avl))
	{
		is_good = 1;
	}
	TEST("TestBSTInsert", is_good, 1);
	AVLDestroy(avl);
}

static void TestAVLRemove(void)
{
    avl_node_t* avl = AVLCreate(CmpFunc);
    int arr[] = {1, 18, 9, 6, 12, 15};
    int b[] = {1, 6, 9, 12, 15, 18};
    size_t size = sizeof(arr) / sizeof(int);
    size_t i = 0;
    size_t is_good = 1;
    void* data = NULL;
    
    for(; i < size; ++i)
    {
        AVLInsert(avl, &arr[i]);
    }
    
    for(i = 0; i < size - 1; ++i)
    {
        data = AVLRemove(avl, b[i]);
        
        if(b[i] != data)
        {
            is_good = 0; 
        }
    }
    
    TEST("TestAVLRemove", is_good, 1);
    
    AVLDestroy(avl);
}



int main() 
{
    
    
    printf("\nSUMMARY: %lu/%lu tests passed.\n", tests_passed, tests_run);
    return 0;
}
